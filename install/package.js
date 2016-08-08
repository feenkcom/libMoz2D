#!/usr/bin/env node
/**
 * Created by Aliaksei Syrel on 07/08/16.
 */

var Parser = require('./parser.js');

module.exports = function Package(_args) {
    var _this = this;

    var name;
    var makefile;
    var sourcesPath;
    var objectsPath;

    var library;
    var defines;
    var sources;
    var unifiedSources;
    var includes;
    var targetFlags;
    var sourceFlags;

    _this.initialize = function (args) {
        name = args.name;
        makefile = args.makefile;
        sourcesPath = args.sources;
        objectsPath = args.objects;

        var props = new Parser({
            file: makefile,
            package: name,
            sources: sourcesPath,
            objects: objectsPath
        }).parse();

        library = props.library;
        defines = props.defines;
        sources = props.sources;
        unifiedSources = props.unifiedSources;
        includes = props.includes;
        targetFlags = props.targetFlags;
        sourceFlags = props.sourceFlags;
    };

    _this.name = function () {
        return name;
    };

    _this.makefile = function () {
        return makefile;
    };

    _this.library = function () {
        return library;
    };

    _this.defines = function () {
        return defines;
    };

    _this.sources = function () {
        return sources;
    };

    _this.unifiedSources = function () {
        return unifiedSources;
    };

    _this.includes = function () {
        return includes;
    };

    _this.targetFlags = function () {
        return targetFlags;
    };

    _this.sourceFlags = function () {
        return sourceFlags;
    };

    _this.fullSourcePath = function () {
        return sourcesPath + '/' + name;
    };

    _this.fullObjectPath = function () {
        return objectsPath + '/' + name;
    };

    /**
     * A visitor pattern
     * @param visitor
     * @return {*}
     */
    _this.accept = function (visitor) {
        return visitor.visitPackage(_this);
    };

    _this.initialize(_args);
};