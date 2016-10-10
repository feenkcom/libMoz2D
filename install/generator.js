/**
 * Created by Aliaksei Syrel on 08/08/16.
 */

var fs = require('fs');
_ = require('./libs/underscore.js');
var Platform = require('./platform.js');

CMAKE_FILE = 'CMakeLists.txt';

module.exports = function Generator() {
    var _this = this;

    _this.visitPackage = function (aPackage) {
        var cmake = _this.generatePackage(aPackage);
        _this.write(aPackage.fullObjectPath() + '/' + CMAKE_FILE, cmake);
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
        
        var toExclude = _.filter(aPackage.sources(), function (source) {
            return _.some(Platform.getPlatform().excludes(), function (exclude) {
            	return (aPackage.fullSourcePath()+'/'+ source).indexOf(exclude) !== -1;
            });        
        });
        
        if (toExclude.length > 0) {
        		result = _(toExclude).reduce(function(memo, source){
            	return memo + ' "${PROJECT_SOURCE_DIR}/'+aPackage.fullSourcePath()+'/'+source +'"';
        		}, result + 'list(REMOVE_ITEM ' + _this.generatePackageSourceVariable(aPackage)) + ')\n';
        }
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
            return memo + flag.replace(/"/g, '\\"') + ' '; }, result);
        return result +'")\n';
    };

    _this.generatePackageSourceProperties = function (aPackage) {
        return _(aPackage.sourceFlags()).reduce(function(memo, flags, source){
            return memo + 'set_source_files_properties(${PROJECT_SOURCE_DIR}/'+aPackage.fullSourcePath()+'/'+source+' PROPERTIES COMPILE_FLAGS "'+flags+'")\n'; }, '');
    };

    _this.generate = function (anObject) {
        return anObject.accept(this);
    };

    _this.visitPlatform = function (aPlatform) {
        var cmake = _this.generatePlatformCmakeVersion(aPlatform) + '\n';
        cmake += _this.generatePlatformCCompiler(aPlatform);
        cmake += _this.generatePlatformCXXCompiler(aPlatform) + '\n';
        cmake += _this.generatePlatformProjectName(aPlatform) + '\n';
        cmake += _this.generatePlatformMajorVersion(aPlatform);
        cmake += _this.generatePlatformMinorVersion(aPlatform) + '\n';
        cmake += _this.generatePlatformMozPathVariables(aPlatform) + '\n';
        cmake += _this.generatePlatformEnableAsm() + '\n';
        cmake += _this.generatePlatformDefines(aPlatform) + '\n';
        cmake += _this.generatePlatformAsmFlags(aPlatform);
        cmake += _this.generatePlatformCFlags(aPlatform);
        cmake += _this.generatePlatformCxxFlags(aPlatform);
        cmake += _this.generatePlatformLinkerFlags(aPlatform) + '\n';
        cmake += _this.generatePlatformIncludes(aPlatform) + '\n';
        cmake += _this.generatePlatformPackages(aPlatform) + '\n';
        cmake += _this.generatePlatformSourcesFlags(aPlatform) + '\n';
        cmake += _this.generatePlatformLibraries(aPlatform);
        cmake += _this.generatePlatformLinkLibraries() + '\n';
        _this.write(CMAKE_FILE, cmake);
        return cmake;
    };

    _this.generatePlatformCmakeVersion = function(aPlatform) {
        return 'cmake_minimum_required (VERSION '+ aPlatform.cmakeVersion().toFixed(1)+')\n';
    };

    _this.generatePlatformCCompiler = function (aPlatform) {
        return 'set (CMAKE_C_COMPILER "'+ aPlatform.cCompiler() +'")\n'
    };

    _this.generatePlatformCXXCompiler = function (aPlatform) {
        return 'set (CMAKE_CXX_COMPILER "'+ aPlatform.cxxCompiler() +'")\n'
    };

    _this.generatePlatformProjectName = function (aPlatform) {
        return 'project ('+ aPlatform.projectName() +')\n';
    };

    _this.generatePlatformMajorVersion = function(aPlatform) {
        return 'set ('+ aPlatform.projectName() +'_VERSION_MAJOR '+ aPlatform.versionMajor()+')\n';
    };

    _this.generatePlatformMinorVersion = function(aPlatform) {
        return 'set ('+ aPlatform.projectName() +'_VERSION_MINOR '+ aPlatform.versionMinor()+')\n';
    };

    _this.generatePlatformEnableAsm = function () {
        return 'enable_language(ASM)\n';
    };

    _this.generatePlatformMozPathVariables = function(aPlatform) {
        var result = 'set (MOZ_TOP_PATH ${PROJECT_SOURCE_DIR}/'+aPlatform.sources()+' CACHE STRING "Path to mozilla-central repo")\n';
        result += 'set (MOZ_TOP_OBJ_PATH ${PROJECT_SOURCE_DIR}/'+aPlatform.objects()+' CACHE STRING "Path to object directory")\n';
        return result;
    };

    _this.generatePlatformDefines = function(aPlatform) {
        return 'add_definitions('+aPlatform.defines()+')\n';
    };

    _this.generatePlatformAsmFlags = function (aPlatform) {
        return 'set (CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} '+aPlatform.asmFlags()+'")\n';
    };

    _this.generatePlatformCFlags = function (aPlatform) {
        return 'set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ' + aPlatform.cCompilerFlags() + ' ' + aPlatform.cFlags()+'")\n';
    };

    _this.generatePlatformCxxFlags = function (aPlatform) {
        return 'set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ' + aPlatform.cxxCompilerFlags() + ' ' + aPlatform.cxxFlags() + '")\n';
    };

    _this.generatePlatformLinkerFlags = function (aPlatform) {
        return 'set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} '+ aPlatform.linkerFlags() +'")\n';
    };

    _this.generatePlatformIncludes = function (aPlatform) {
        return _this.merge(aPlatform.includes(), '\n', function(include){
            return 'include_directories('+ include+ ')';
        }) + '\n';
    };

    _this.generatePlatformPackages = function (aPlatform) {
        var subdirs = _(aPlatform.packages()).reduce(function(memo, pkg) {
            return memo + '     add_subdirectory(${MOZ_TOP_OBJ_PATH}/'+pkg.name()+')\n'
        }, '');
        subdirs = _(aPlatform.bindingSources()).reduce(function(memo, name) {
            return memo + '     add_subdirectory('+name+')\n'
        }, subdirs + '\n');

        var library = _(aPlatform.packages()).reduce(function(memo, pkg) {
            return memo + '     $<TARGET_OBJECTS:'+pkg.library()+'>\n'
        }, 'add_library(${PROJECT_NAME} SHARED\n');
        library = _(aPlatform.bindingPackages()).reduce(function(memo, name) {
            return memo + '     $<TARGET_OBJECTS:'+name+'>\n'
        }, library + '\n');
        library += ')\n';
        return subdirs + '\n' + library;
    };

    _this.generatePlatformSourcesFlags = function (aPlatform) {
        return _this.merge(aPlatform.sourcesFlags(), '\n', function(obj) {
            return 'set_source_files_properties(' + obj.source + ' PROPERTIES COMPILE_FLAGS "' + obj.flags + '")';
        }) + '\n';
    };

    _this.generatePlatformLibraries = function (aPlatform) {
        return _this.merge(aPlatform.libraries(), '\n', function(lib) {
            return 'set(LIBRARIES ${LIBRARIES} "'+ lib +'")';
        }) + '\n';
    };

    _this.generatePlatformLinkLibraries = function () {
        return 'target_link_libraries(${PROJECT_NAME} ${LIBRARIES})\n';
    };

    _this.visitConfig = function (aConfig) {
        var platform = Platform.getPlatform();
        var conf = _this.generateConfigCrossCompile(platform);
        conf += _this.generateConfigObjDir(platform);
        conf += _this.generateConfigExports(platform) + '\n';
        conf += _this.generateConfigMakeFlags(platform) + '\n';
        conf += _this.generateConfigOptions(platform) + '\n';
        conf += _this.generateConfigModules(platform);
        _this.write(platform.mozconfigPath(), conf);
        return conf;
    };

    _this.generateConfigCrossCompile = function (aPlatform) {
        return aPlatform.crossCompile() ? 'export CROSS_COMPILE=1\n\n' : '';
    };

    _this.generateConfigObjDir = function (aPlatform) {
        return 'mk_add_options MOZ_OBJDIR=@TOPSRCDIR@/' + aPlatform.config().build.objects + '\n';
    };

    _this.generateConfigMakeFlags = function (aPlatform) {
        return 'mk_add_options MOZ_MAKE_FLAGS="' + aPlatform.configFlags() + '"\n';
    };
    
    _this.generateConfigOptions = function (aPlatform) {
		return _this.merge(aPlatform.configOptions(), '\n', function(option){
			return 'ac_add_options ' + option;
		}) + '\n';
    };
	
	_this.generateConfigExports = function (aPlatform) {
		return _this.merge(aPlatform.configExports(), '\n', function(each){
			return 'export ' + each;
		}) + '\n';
    };

    _this.generateConfigModules = function (aPlatform) {
        var enabled = _this.merge(aPlatform.enabledModules(), '\n', function(item) {
            return 'ac_add_options --enable-' + item;
        })+'\n';
        var disabled = _this.merge(aPlatform.disabledModules(), '\n', function(item) {
            return 'ac_add_options --disable-' + item;
        })+'\n';
        return enabled + '\n' + disabled;
    };

    _this.merge = function(array, _delimiter, _transform) {
        var delimiter = _.isUndefined(_delimiter) ? ' ' : _delimiter;
        var transform = _.isUndefined(_transform) ? _.identity : _transform;

        return _(array).reduce(function(memo, each, index) {
            return memo + (index > 0 ? delimiter : '') + transform(each);
        }, '');
    };

    _this.write = function (file, content) {
        fs.writeFileSync(file,content);
    };

    _this.read = function (file) {
        var contents;
        try {
            fs.statSync(file);
            contents = fs.readFileSync(file);
        }
        catch (e) {
            contents = null;
        }
        return contents;
    };
};