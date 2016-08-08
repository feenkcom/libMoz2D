/**
 * Created by Aliaksei Syrel on 08/08/16.
 */


fs = require('fs');
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

    _this.download = function () {
        platform.log('Downloading sources...');
        if (_this.isDownloaded()) {
            platform.log('   Achieve exists, no need to download. [perform clean for fresh install]');
            return;
        }
        var cmd = 'wget ' + platform.sourcesURL() + ' -O' + platform.sourcesArchive()+'_tmp';
        execSync(cmd, { stdio: ['pipe', 'pipe', process.stderr] });
        execSync('mv -fv '+platform.sourcesArchive()+'_tmp ' + platform.sourcesArchive(), { stdio: ['pipe', 'pipe', process.stderr] });
        platform.log('Downloaded as ' + platform.sourcesArchive());
    };

    _this.extract = function () {
        platform.log('Extracting ' + platform.sourcesArchive() + '... this may take a while');
        if (!_this.isDownloaded())
            throw new Error('Error! Sources are not downloaded!');

        if (_this.isExtracted()) {
            platform.log('   Sources exist, no need to extract. [perform clean for fresh install]');
            return;
        }

        _this.exec('rm -rf ' + platform.sources() + '_tmp');
        _this.exec('mkdir ' + platform.sources() + '_tmp && tar -zxf ' + platform.sourcesArchive() +' -C ' + platform.sources() + '_tmp --strip-components 1');
        _this.exec('rm -rf ' + platform.sources());
        _this.exec('mv ' + platform.sources() + '_tmp ' + platform.sources());
        platform.log('Extracted in ' + platform.sources());
    };

    _this.patch = function () {
        platform.log('Applying patches...');
        _(platform.patches()).each(function(patch) {
            try {
                _this.exec('patch --forward -p0 -d ' + platform.sources() +' < ' + patch);
            }
            // error means that patch was already applied, so it is ok to just print message
            catch(e) {
                //console.error(e.stdout.toString());
            }
        });
    };

    _this.configure = function () {
        if (_this.isConfigured()) {
            platform.log('Build system already configured, no need to configure.');
            return;
        }
        _this.exec('rm -rf ' + _this.configCheckFile());


        platform.log('Generating ' + platform.mozconfigPath() + '...');
        new Generator().generate(platform.config());
        platform.log('   Done');

        platform.log('Configuring build system...');
        _this.exec('./mach clobber', platform.sources());
        _this.exec('./mach configure', platform.sources());
        platform.log('   Done');

        platform.log('Generating ipdl sources...');
        _this.exec('make recurse_pre-export', platform.objects());
        _this.exec('make mozilla-config.h buildid.h source-repo.h', platform.objects());

        var mozillaConfig = fs.openSync(platform.mozillaConfigH(), 'a');
        _(platform.undefines()).each(function(each) {
            fs.writeSync(mozillaConfig, '#undef ' + each);
        });
        fs.closeSync(mozillaConfig);

        _this.exec('make recurse_export', platform.objects());
        platform.log('   Done');

        new Generator().write(_this.configCheckFile(), JSON.stringify(platform.config()));
    };

    _this.export = function () {
        platform.log('Parsing makefiles...');
        _(platform.packages()).each(function(pkg) {
            new Generator().generate(pkg);
            platform.log('Converted ' + pkg.fullObjectPath() + '/' + pkg.makefile() + ' (' + pkg.library() + ')');
        });
        new Generator().generate(platform);
        platform.log('Generated root CMakeLists.txt');
        platform.log('   Done');
    };

    _this.compile = function () {
        throw new Error('Subclass responsibility!');
    };

    _this.isDownloaded = function () {
        return _this.isExists(platform.sourcesArchive());
    };

    _this.isExtracted = function () {
        return _this.isExists(platform.sources());
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

    _this.exec = function(cmd, dir) {
        var cwd = _.isUndefined(dir) ? dir : execSync('pwd').toString().trim() + '/' + dir;
        return execSync(cmd, { stdio: 'inherit', cwd: cwd });
    };

    _this.configCheckFile = function () {
        return platform.objects() + '/' + '.mozconfig.json';
    };

    _this.initialize(_args);
}

module.exports = Builder;