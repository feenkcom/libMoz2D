/**
 * Created by Aliaksei Syrel on 09/08/16.
 */

Platform = require('./platform.js');
Builder = require('./builder.js');

function BuilderMac(args) {
    var _this = new Builder(args);

    _this.compile = function () {
        _this.platform().log('Compiling shared library...');
        _this.exec('mkdir -p ' + _this.platform().output());
        _this.exec('cmake -DCMAKE_OSX_ARCHITECTURES='+ _this.platform().arch() +' ..', _this.platform().output());
        _this.exec('make',_this.platform().output());
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

    return _this;
}

module.exports = PlatformMac;