/**
 * Created by Aliaksei Syrel on 10/08/16.
 */

Platform = require('./platform.js');
Builder = require('./builder.js');

function BuilderWin(args) {
    var _this = new Builder(args);

    _this.compile = function () {
        _this.platform().log('Compiling shared library...');
        _this.exec('mkdir -p ' + _this.platform().output());
        _this.exec('cmake ..', _this.platform().output());
        _this.exec('make',_this.platform().output());
    };

    return _this;
}

function PlatformWin() { // subclass Platform
    var _this = new Platform();

    /**
     * Return a platform specific object field for my platform
     * @param anObject
     */
    _this.platform = function (anObject) {
        return anObject.win;
    };

    /**
     * Creates a platform dependent builder
     * @return {BuilderWin}
     */
    _this.builder = function () {
        return new BuilderWin({platform: _this});
    };

    return _this;
}

module.exports = PlatformWin;