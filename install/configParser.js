/**
 * Created by Aliaksei Syrel on 09/08/16.
 */

Platform = require('./platform.js');
fs = require('fs');
_ = require('./libs/underscore.js');

OS_CXXFLAGS = 'OS_CXXFLAGS';
OS_CFLAGS = 'OS_CFLAGS';
OS_COMPILE_CXXFLAGS = 'OS_COMPILE_CXXFLAGS';
OS_COMPILE_CFLAGS = 'OS_COMPILE_CFLAGS';
ASFLAGS = 'ASFLAGS';
CC = 'CC';
CXX = 'CXX';

function ConfigStatusParser() {
    var _this = this;

    var contents;

    _this.initialize = function () {
        contents = _this.readConfig();
    };

    _this.parseVar = function (type) {
        var flags = _(contents).find(function(line) {
            return line.trim().startsWith('"' + type + '"');
        });
        if (_.isUndefined(flags))
            throw new Error (type + ' not found!');
        flags = _this.trimAfter(flags, type+'":');

        // removes commas from the begin and end of the string
        flags = flags.replace (/(^,)|(,$)/g, '');
        // removes double quotes from the begin and end
        flags = flags.replace (/(^")|("$)/g, '');

        var replaces = {
            '$(topsrcdir)': '${PROJECT_SOURCE_DIR}/' + Platform.getPlatform().sources(),
            '$(topobjdir)': '${PROJECT_SOURCE_DIR}/'+ Platform.getPlatform().objects()
        };

        flags = _this.replace(flags, replaces);
        return flags;
    };

    _this.parseCXXFlags = function () {
        return _this.parseVar(OS_CXXFLAGS);
    };

    _this.parseCFlags = function () {
        return _this.parseVar(OS_CFLAGS);
    };

    _this.parseCompileCXXFlags = function () {
        return _this.parseVar(OS_COMPILE_CXXFLAGS);
    };

    _this.parseCompileCFlags = function () {
        return _this.parseVar(OS_COMPILE_CFLAGS);
    };

    _this.parseASMFlags = function () {
        return _this.parseVar(ASFLAGS);
    };

    _this.parseCCompiler = function () {
        var compiler = _this.parseVar(CC);
        compiler = compiler.split(' ');
        if (compiler.length < 1)
            throw new Error('C Compiler not defined!');
        return compiler[0].trim();
    };

    _this.parseCCompilerFlags = function () {
        var compiler = _this.parseVar(CC);
        compiler = compiler.split(' ');
        compiler = compiler.slice(1, compiler.length);
        return _(compiler).reduce(function(memo, each) {
            return memo + ' ' + each;
        }, '');
    };

    _this.parseCXXCompiler = function () {
        var compiler = _this.parseVar(CXX);
        compiler = compiler.split(' ');
        if (compiler.length < 1)
            throw new Error('CXX Compiler not defined!');
        return compiler[0].trim();
    };

    _this.parseCXXCompilerFlags = function () {
        var compiler = _this.parseVar(CXX);
        compiler = compiler.split(' ');
        compiler = compiler.slice(1, compiler.length);
        return _(compiler).reduce(function(memo, each) {
            return memo + ' ' + each;
        }, '');
    };

    /**
     * Reads config.status file and returns its content splitted by new line
     * @return {Array|{index: number, input: string}}
     */
    _this.readConfig = function () {
        var data = fs.readFileSync(_this.fullConfigPath(), 'utf8');
        return data.match(/[^\r\n]+/g);
    };

    _this.fullConfigPath = function () {
        return  Platform.getPlatform().objects() + '/' +  Platform.getPlatform().configStatus();
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

    _this.initialize();
}

module.exports = ConfigStatusParser;