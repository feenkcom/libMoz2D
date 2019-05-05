/**
 * Created by Aliaksei Syrel on 08/08/16.
 */


fs = require('fs');
os = require('os');

var execSync = require('child_process').execSync;
_ = require('./libs/underscore.js');
var Generator = require('./generator.js');

function Builder (_args) {
    var _this = this;

    var platform;

    _this.initialize = function(args) {
        platform = args.platform;
    };

    /**
     * Return a currently bounded platform
     * @return {Platform}
     */
    _this.platform = function () {
        return platform;
    };

    /**
     * Downloads latest mozilla sources from repository
     */
    _this.download = function () {
        _this.stage('Downloading sources...');
        if (_this.isDownloaded()) {
            platform.log('   Archive exists, no need to download');
            return;
        }
        if (_this.isExtracted()) {
            _this.log(_this.tab('Sources exist, no need to download'));
            return;
        }
        // --no-check-certificate is required because on windows it fails to locally verify issuer's authority
        var cmd = 'wget -q --no-check-certificate ' + platform.sourcesURL() + ' -O' + platform.sourcesArchive()+'_tmp';
        execSync(cmd, { stdio: ['pipe', 'pipe', process.stderr] });
        execSync('mv -fv '+platform.sourcesArchive()+'_tmp ' + platform.sourcesArchive(), { stdio: ['pipe', 'pipe', process.stderr] });
        platform.log(_this.tab('Downloaded as ' + platform.sourcesArchive()));
    };

    /**
     * Extracts mozilla sources from archive
     */
    _this.extract = function () {
        _this.stage('Extracting ' + platform.sourcesArchive() + '... this may take a while');

        if (_this.isExtracted()) {
            _this.log(_this.tab('Sources exist, no need to extract.'));
            return;
        }
		
        if (!_this.isDownloaded())
            throw new Error('Error! Sources are not downloaded!');

        _this.exec('rm -rf ' + platform.sources() + '_tmp');
        _this.exec('mkdir ' + platform.sources() + '_tmp');

        try{
			var command = 'tar -zxvf ' + platform.sourcesArchive() +' -C ' + platform.sources() + '_tmp --strip-components 1 2>&1 | awk \'!(NR%5000)\'';
			_this.exec(command);
			
        } catch(e) {
            platform.log('	We continue, sometimes tar has issues creating symlinks, it is not relevant.');
        }

        _this.exec('rm -rf ' + platform.sources());
        _this.exec('mv ' + platform.sources() + '_tmp ' + platform.sources());
        platform.log('Extracted in ' + platform.sources());
    };

    /**
     * Applies patches on mozilla sources
     */
    _this.patch = function () {
        _this.stage('Applying patches...');

        var tryPatches = function () {
            return _.chain(platform.patches()).map(function(patch) {
                var status = 'unknown';
                var output = '';
                try {
                    if (_this.isPatchApplied(patch)) {
                        status = 'applied';
                        output = patch + ' is already applied';
                    }
                    else {
                        output = (_this.execSilent('patch --dry-run --forward --ignore-whitespace -p0 -d ' + platform.sources() +' < ' + patch)).toString();
                        status = 'success';
                    }
                }
                catch(e) {
                    output = e.stdout.toString();
                    status = _this.isPatchApplied(patch) ? 'applied' : 'fail';
                }
                return {
                    patch: patch,
                    status: status,
                    output: output
                }
            }).groupBy(function(result) {
                    return result.status;
                }).value();
        };

        var results = tryPatches();

        if (!_.isUndefined(results.fail) && results.fail.length > 0) {
            _(results.fail).each(function(fail) {
                _this.error(fail.patch);
                _this.log(_this.tab(fail.output));
            });
            _this.log('Successful patches:');
            _(results.success).each(function(success) {
                _this.success(_this.tab(success.patch));
            });
            _this.log('Failed patches: (read log above to know why)');
            _(results.fail).each(function(fail) {
                _this.error(_this.tab(fail.patch));
            });

            process.exit(1);
        }

        if (!_.isUndefined(results.applied) && results.applied.length > 0) {
            _this.log("Already applied patches:");
            _(results.applied).each(function(applied) {
                _this.info(_this.tab(applied.patch));
            });
        }

        _(results.success).each(function(success) {
            try {
                _this.execSilent('patch --force --ignore-whitespace -p0 -d ' + platform.sources() +' < ' + success.patch);
                _this.execSilent('cp -f ' + success.patch + ' ' + _this.patchCheckFile(success.patch));
                _this.success(_this.tab(success.patch));
            }
            catch(e) {
                _this.log();
                _this.error(_this.tab("Unexpected patch failed: " + success.patch));
                process.exit(1);
            }
        });
    };

    _this.configure = function () {
        if (_this.isConfigured()) {
            platform.log('Build system already configured, no need to configure.');
            return;
        }
        _this.exec('rm -rf ' + _this.configCheckFile());

        _this.stage('Generating ' + platform.mozconfigPath() + '...');
        new Generator().generate(platform.config());
        _this.success(_this.tab('Done'));

        _this.stage('Configuring build system...');
        _this.exec('sh mach clobber', platform.sources());
        _this.exec('sh mach configure', platform.sources());
        _this.success(_this.tab('Done'));

        _this.stage('Generating ipdl sources...');
        _this.make('recurse_pre-export', platform.objects());
        _this.make('mozilla-config.h buildid.h source-repo.h', platform.objects());

        _this.stage('Undefining defines...');
        var mozillaConfig = fs.openSync(platform.mozillaConfigH(), 'a');
        _(platform.undefines()).each(function(each) {
            fs.writeSync(mozillaConfig, '#undef ' + each + '\n');
            _this.log(_this.tab('#undef '+ each));
        });
        fs.closeSync(mozillaConfig);

        _this.make('recurse_export', platform.objects());
        _this.success(_this.tab('Done'));

        new Generator().write(_this.configCheckFile(), JSON.stringify(platform.config()));
    };
	
	/**
	 *	Override in order to use different `make`
	 */
	_this.make = function(target, directory) {
		_this.exec('make ' + target, directory);
	};

    _this.export = function () {
        _this.stage('Parsing makefiles...');
        _(platform.packages()).each(function(pkg) {
            new Generator().generate(pkg);
            _this.log(_this.tab('Converted ' + pkg.fullObjectPath() + '/' + pkg.makefile() + ' (' + pkg.library() + ')'));
        });
        var rootCmake = new Generator().generate(platform);
        _this.success(_this.tab('Generated root CMakeLists.txt'));
        _this.log(_this.tab(_this.tab(rootCmake)));
        _this.success(_this.tab('Done'));
    };

    _this.compile = function () {
        throw new Error('Subclass responsibility!');
    };

    _this.isDownloaded = function () {
        return _this.isExists(platform.sourcesArchive());
    };

    _this.isExtracted = function () {
        return _this.isExists(platform.sources()) && _this.isEmpty(platform.sources());
    };

    _this.isConfigured = function () {
        return JSON.stringify(platform.config()) == new Generator().read(_this.configCheckFile());
    };

    _this.isExists = function (aFile) {
        var exists = false;

        try {
            fs.statSync(aFile);
            exists = true;
        }
        catch (e) {
            exists = false;
        }
        return exists;
    };
	
    _this.isEmpty = function (aFile) {
        var empty = false;

        try {
            var files = fs.readdirSync(aFile);
            empty = !files.length;
        }
        catch (e) {
            empty = true;
        }
        return empty;
    };

    _this.exec = function(cmd, dir) {
        // we use shell wrapper to print all errors
        var command = 'sh ' + platform.config().project.installer + '/exec.sh "' + cmd + '"';
        if (!_.isUndefined(dir)) command += ' "' + dir + '"';
        return execSync(command, { stdio: [process.stdin, process.stdout, process.stdout] });
    };

    _this.execSilent = function(cmd, dir) {
        // we use shell wrapper to print all errors
        var command = 'sh ' + platform.config().project.installer + '/exec.sh "' + cmd + '"';
        if (!_.isUndefined(dir)) command += ' "' + dir + '"';
        return execSync(command);
    };

    _this.error = function(message) {
        platform.log('\x1b[31m%s\x1b[0m', message);
    };

    _this.success = function(message) {
        platform.log('\x1b[32m%s\x1b[0m', message);
    };

    _this.info = function (message) {
        platform.log('\x1b[33m%s\x1b[0m', message);
    };

    _this.log = function (message) {
        platform.log(message);
    };

    _this.stage = function (message) {
        platform.log('\x1b[36m%s\x1b[0m', message);
    };

    _this.tab = function (string) {
        return _.chain(string.split(os.EOL)).map(function(line) {
            return "    " + line;
        }).reduce(function(memo, line) {
                return memo + (memo.length > 0 ? os.EOL : "") + line;
            }, "").value();
    };

    _this.isPatchApplied = function (aPatch) {
        return new Generator().read(aPatch).toString() == new Generator().read(_this.patchCheckFile(aPatch).toString());
    };

    _this.configCheckFile = function () {
        return platform.objects() + '/' + '.mozconfig.json';
    }

    _this.patchCheckFile = function (aPatch) {
        var path = require("path");
        var name = path.basename(aPatch);
        return platform.sources() + '/' + name + '_tmp';
    };

    _this.initialize(_args);
}

module.exports = Builder;