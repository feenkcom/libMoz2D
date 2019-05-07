/**
 * Created by Aliaksei Syrel on 09/08/16.
 */

Platform = require('./platform.js');
Builder = require('./builder.js');

_ = require('./libs/underscore.js');
override = require('./libs/override.js');

function BuilderMac(args) {
    var _this = new Builder(args);

    _this.compile = function () {
        _this.platform().log('Compiling shared library...');
        _this.exec('mkdir -p ' + _this.platform().output());
        _this.exec('cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES='+ _this.platform().arch() +' ..', _this.platform().output());
        _this.exec('make',_this.platform().output());
    };
	
    /**
     * Execute a command to extract sources
     */
    _this.execExtract = function (command) {
        _this.execSilent(command);
    };

    return _this;
}

function PlatformMac() { // subclass Platform
    var _this = new Platform();

    /**
     * Return a platform specific object field for my platform
     * @param anObject
     */
    _this.platform = function (anObject) {
        return anObject.mac;
    };

    /**
     * Creates a platform dependent builder
     * @return {BuilderMac}
     */
    _this.builder = function () {
        return new BuilderMac({platform: _this});
    };

    /**
     * On OSX there are no libraries, they are Frameworks
     * and have to be defined as linker flags in form
     * -framework %name%
     * @return {Array}
     */
    _this.libraries = function () {
        return [];
    };

    _this.linkerFlags = override(_this, _this.linkerFlags, function() {
        return _.reduce(_this.platformLibraries(), function(memo, lib) {
            return memo + ' -framework ' + lib;
        }, this.super());
    });

    return _this;
}

module.exports = PlatformMac;