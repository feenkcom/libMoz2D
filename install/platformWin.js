/**
 * Created by Aliaksei Syrel on 10/08/16.
 */

Platform = require('./platform.js');
Builder = require('./builder.js');
override = require('./libs/override.js');

function BuilderWin(args) {
    var _this = new Builder(args);

	_this.patch = override(_this, _this.patch, function() {
		_this.stage('Converting patches...');
        _this.exec('find ' + _this.platform().patchesPath() +' -type f -print0 | xargs -0 dos2unix');
        _this.success(_this.tab('Done'));
        this.super();
    });

    _this.compile = function () {
        _this.platform().log('Compiling shared library...');
        _this.exec('mkdir -p ' + _this.platform().output());
        _this.exec('cmake .. -G \'Visual Studio 14 2015\'', _this.platform().output());
        _this.exec('cmake --build . --target Moz2D --config Release',_this.platform().output());
    };
	
	_this.make = function(target, directory) {
		_this.exec('mozmake ' + target, directory);
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
	
	_this.libraries = function () {
        return _this.platformLibraries();
    };

    return _this;
}

module.exports = PlatformWin;