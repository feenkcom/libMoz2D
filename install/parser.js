#!/usr/bin/env node
/**
 * Created by Aliaksei Syrel on 07/08/16.
 */

fs = require('fs');
_ = require('./libs/underscore.js');

DEFINES = 'DEFINES';
LIBRARY_NAME = 'LIBRARY_NAME';
LOCAL_INCLUDES = 'LOCAL_INCLUDES';
SOURCES = ['CSRCS', 'CPPSRCS', 'ASFILES', 'CMMSRCS'];
TARGET_FLAGS = ['MOZBUILD_CXXFLAGS', 'MOZBUILD_CFLAGS', 'MOZBUILD_ASFLAGS'];
/**
 *
 * @param {Object} _args - arguments used to create parser
 * @param {String} _args.file - file (backend.mk) to parse
 * @constructor
 */
module.exports = function Parser(_args) {
    var _this = this;

    var filename;
    var packagePath;
    var sourcePath;
    var objectPath;

    var contents;

    _this.initialize = function(args) {
        filename = args.file;
        sourcePath = args.sources;
        objectPath = args.objects;
        packagePath = args.package;
        contents = _this.readFile();
    };

    _this.parse = function() {
        return {
            'library': _this.parseLibName(),
            'defines': _this.parseDefines(),
            'sources': _this.parseSources(),
            'unifiedSources': _this.parseUnifiedSources(),
            'includes': _this.parseLocalIncludes(),
            'targetFlags': _this.parseTargetFlags(),
            'sourceFlags': _this.parseSourceFlags()
        };
    };

    /**
     * Return an array of DEFINES if any or empty array
     * @return {Array}
     */
    _this.parseDefines = function () {
        return _.chain(contents).filter(function(line) {
            return line.trim().startsWith(DEFINES);
        }).map(function(line) {
            return _this.trimAfter(line,'+=').split(' ');
        }).flatten().value();
    };

    /**
     * Return a library name
     * @return {String}
     * @throws Error if library name is not specified in file
     */
    _this.parseLibName = function () {
        var libName =  _(contents).find(function(line) { return line.trim().startsWith(LIBRARY_NAME)} );
        if (_.isUndefined(libName))
            throw new Error('Library names is not specified!');
        return _this.trimAfter(libName, ':=');
    };

    /**
     * Return an array of all sources of all known types (.c .cpp .mm .as)
     * @return {Array}
     */
    _this.parseSources = function () {
        return _.chain(contents).filter(function(line) {
            return _(SOURCES).some(function(type) {
                return line.trim().startsWith(type + ' +=');
            });
        }).map(function(line) {
            return _this.trimAfter(line, '+=');
        }).filter(function(source){
            return !source.startsWith('$');
        }).value();
    };

    /**
     * Return an array of local includes
     * @return {Array|String}
     */
    _this.parseLocalIncludes = function () {
        var includes =  _.chain(contents).filter(function(line) {
            return line.trim().startsWith(LOCAL_INCLUDES)
        }).map(function(line) {
            return _this.trimAfter(line, '+= -I');
        });

        var replaces = {
            '$(topsrcdir)': sourcePath,
            '$(topobjdir)': objectPath,
            '$(srcdir)': _this.fullSourcePath(),
            '$(CURDIR)': _this.fullObjectPath()
        };

        includes = includes.concat([
            objectPath + '/'+ packagePath,
            sourcePath + '/'+ packagePath
        ]);

        return includes.map(function(include) {
            return _this.replace(include, replaces);
        }).value();
    };

    /**
     * Return an array of unified sources
     * @return {Array}
     */
    _this.parseUnifiedSources = function () {
        return _.chain(contents).filter(function(line) {
            return _(SOURCES).some(function(type) {
                return line.trim().startsWith('UNIFIED_'+type);
            });
        }).map(function(line) {
            return _this.trimAfter(line, ':=').split(' ');
        }).flatten().value();
    };

    /**
     * Return an array containing target flags
     * @return {Array|String}
     */
    _this.parseTargetFlags = function () {
        return _.chain(contents).filter(function(line) {
            return _(TARGET_FLAGS).some(function(type) {
                return line.trim().startsWith(type + ' +=');
            });
        }).map(function(line) {
            return _this.trimAfter(line, '+=').split(',');
        }).flatten().value();
    };

    /**
     * Return an association of source => compile flags
     * @return {Object}
     */
    _this.parseSourceFlags = function () {
        var sources = _this.parseSources();
        return _.chain(contents).filter(function(line) {
            return _.chain(sources).some(function(source){
                return line.trim().startsWith(source +'_FLAGS');
            }).value();
        }).map(function(line){
            return line.trim().split('_FLAGS += ');
        }).object().value();
    };

    /**
     * Reads file and returns its content splitted by new line
     * @return {Array|{index: number, input: string}}
     */
    _this.readFile = function () {
        var data = fs.readFileSync(_this.fullFilePath(), 'utf8');
        return data.match(/[^\r\n]+/g);
    };

    /**
     * Return full file path in form form 'mozilla-central/obj-mozilla/gfx/2d/backend.mk
     * @return {string}
     */
    _this.fullFilePath = function () {
        return objectPath + '/' + packagePath + '/' + filename;
    };

    /**
     * Return full source path in form 'mozilla-central/gfx/2d
     * @return {string}
     */
    _this.fullSourcePath = function () {
        return sourcePath + '/' + packagePath;
    };

    /**
     * Return full object path in form 'mozilla-central/obj-mozilla/gfx/2d
     * @return {string}
     */
    _this.fullObjectPath = function () {
        return objectPath + '/' + packagePath;
    };

    /**
     * Return a trimmed substring of a string after a prefix
     * @param {String} string
     * @param {string} prefix
     * @return {string}
     */
    _this.trimAfter = function(string, prefix) {
        return string.substring(string.indexOf(prefix) + prefix.length, string.length).trim();
    };

    /**
     * Replaces substrings from a string by ones defines in a table
     * @param string
     * @param table
     */
    _this.replace = function (string, table) {
        var result = string;
        _(table).each(function(to, what) {
            result = result.replace(what,to);
        });
        return result;
    };

    _this.initialize(_args);
};