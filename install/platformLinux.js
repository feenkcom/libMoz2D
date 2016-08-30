/**
 * Created by Aliaksei Syrel on 09/08/16.
 */

Platform = require('./platform.js');
Builder = require('./builder.js');

function BuilderLinux(args) {
    var _this = new Builder(args);

    _this.compile = function () {
        _this.platform().log('Compiling shared library...');
        _this.exec('mkdir -p ' + _this.platform().output());
        _this.exec('cmake ..', _this.platform().output());
        _this.exec('make',_this.platform().output());
    };

    return _this;
}

function PlatformLinux() { // subclass Platform
    var _this = new Platform();

    /**
     * Return a platform specific object field for my platform
     * @param anObject
     */
    _this.platform = function (anObject) {
        return anObject.linux;
    };

    /**
     * Creates a platform dependent builder
     * @return {BuilderLinux}
     */
    _this.builder = function () {
        return new BuilderLinux({platform: _this});
    };

    _this.libraries = function () {
        return _this.platformLibraries();
    };

    return _this;
}

module.exports = PlatformLinux;
