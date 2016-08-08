/**
 * Created by Aliaksei Syrel on 08/08/16.
 */

var fs = require('fs');
_ = require('./libs/underscore.js');

CMAKE_FILE = 'CMakeLists.txt';

module.exports = function Generator() {
    var _this = this;

    _this.visitPackage = function (aPackage) {
        var cmake = _this.generatePackage(aPackage);
        _this.write(aPackage.fullObjectPath() + '/' + CMAKE_FILE, cmake);
        console.log("Converted: " + aPackage.fullObjectPath() + '/' + aPackage.makefile());
        return cmake;
    };

    _this.generatePackage = function (aPackage) {
        var cmake = "";
        cmake += _this.generatePackageDefines(aPackage) + '\n';
        cmake += _this.generatePackageIncludes(aPackage) + '\n';
        cmake += _this.generatePackageSources(aPackage) + '\n';
        cmake += _this.generatePackageLibrary(aPackage) + '\n';
        cmake += _this.generatePackageTargetProperties(aPackage) + '\n';
        cmake += _this.generatePackageSourceProperties(aPackage) + '\n';
        return cmake;
    };

    _this.generatePackageDefines = function (aPackage) {
        return _(aPackage.defines()).reduce(function(memo, define){
            return memo  + 'add_definitions('+define+')\n'; }, '');
    };

    _this.generatePackageIncludes = function (aPackage) {
        return _(aPackage.includes()).reduce(function(memo, include){
            return memo + 'include_directories(${PROJECT_SOURCE_DIR}/'+include+')\n'; }, '');
    };

    _this.generatePackageSources = function (aPackage) {
        var result = _(aPackage.sources()).reduce(function(memo, source){
            return memo + ' ${PROJECT_SOURCE_DIR}/'+aPackage.fullSourcePath()+'/'+source; }, 'set('+_this.generatePackageSourceVariable(aPackage));
        result = _(aPackage.unifiedSources()).reduce(function(memo, source){
            return memo + ' ${PROJECT_SOURCE_DIR}/'+aPackage.fullObjectPath()+'/'+source; }, result);
        result += ')\n';
        return result;
    };

    _this.generatePackageLibrary = function(aPackage) {
        return 'add_library(' + aPackage.library() + ' OBJECT ${' + _this.generatePackageSourceVariable(aPackage) + '})\n';
    };

    _this.generatePackageSourceVariable = function (aPackage) {
        return aPackage.library() + '_src';
    };

    _this.generatePackageTargetProperties = function (aPackage) {
        var result = 'set_target_properties (' + aPackage.library() +' PROPERTIES COMPILE_FLAGS "';
        result = _(aPackage.targetFlags()).reduce(function(memo, flag){
            return memo + flag+' '; }, result);
        return result +'")\n';
    };

    _this.generatePackageSourceProperties = function (aPackage) {
        return _(aPackage.sourceFlags()).reduce(function(memo, flags, source){
            return memo + 'set_source_files_properties(${PROJECT_SOURCE_DIR}/'+aPackage.fullSourcePath()+'/'+source+' PROPERTIES COMPILE_FLAGS "'+flags+'")\n'; }, '');
    };

    _this.generate = function (anObject) {
        return anObject.accept(this);
    };

    _this.write = function (file, content) {
        fs.writeFileSync(file,content);
    };

};