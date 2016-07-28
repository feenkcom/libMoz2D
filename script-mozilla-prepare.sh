#!/bin/bash

export LC_ALL=C

OPERATION='all'
MOZ_CENTRAL='mozilla-central'

if [ $# -gt 0 ]; then
	if [[ $1 =~ ^priv ]]; then
		echo "Can not execute private functions!" 1>&2
		exit 64
	fi
	OPERATION=$1
fi

if [ $# -gt 1 ]; then
	MOZ_CENTRAL=$2
fi

MOZ_OBJ_DIR="obj-mozilla"

MOZ_TOP_SRC_PATH=$MOZ_CENTRAL
MOZ_TOP_OBJ_PATH=$MOZ_TOP_SRC_PATH"/"$MOZ_OBJ_DIR

MOZ_GFX_DIR='gfx'
	MOZ_2D_DIR='2d'
	MOZ_CAIRO_DIR='cairo'
	MOZ_PIXMAN_DIR='libpixman'
	MOZ_GL_DIR='gl'
	MOZ_LAYERS_DIR='layers'
	MOZ_SKIA_DIR='skia'
	MOZ_THEBES_DIR='thebes'

MOZ_MEMORY_DIR='memory'
	MOZ_MEMORY_BUILD_DIR='build'
	MOZ_FALLIBLE_DIR='fallible'
	MOZ_MEMORY_REPLACE_DIR='replace'
	MOZ_JEMALLOC_DIR='jemalloc'
	MOZ_MOZALLOC_DIR='mozalloc'
	MOZ_MOZJEMALLOC_DIR='mozjemalloc'

MOZ_MFBT_DIR='mfbt'
	
MOZ_INTL_DIR='intl'
	MOZ_UNICHARUTIL_DIR='unicharutil'
	MOZ_UCONV_DIR='uconv'
	
MOZ_IPC_DIR='ipc'

MOZ_XPCOM_DIR='xpcom'
	MOZ_THREADS_DIR='threads'

MOZ_COMMON_DIR='common'
MOZ_DISTDIR='dist'
MOZ_INCLUDEDIR='include'
MOZ_SRC_DIR='src'
MOZ_ICU_DIR='icu'
MOZ_CONFIG_DIR='config'
MOZ_BASE_DIR='base'
MOZ_GLUE_DIR='glue'
MOZ_NSPRPUBDIR='nsprpub'
MOZ_PRDIR='pr'
MOZ_CONFIGDIR='config'
MOZ_EXTERNALDIR='external'
MOZ_INTERNAL_DIR='internal'
MOZ_NSPRDIR='nspr'
MOZ_TESTINGDIR='testing'
MOZ_GTESTDIR='gtest'
MOZ_STRING_DIR='string'
MOZ_STANDALONE_DIR='standalone'
MOZ_UTIL_DIR='util'

MOZ_BACKEND_FILE='backend.mk'

MOZ_GFX_PATH="$MOZ_TOP_SRC_PATH/$MOZ_GFX_DIR"

MOZ_MEMORY_PATH="$MOZ_TOP_SRC_PATH/$MOZ_MEMORY_DIR"
	MOZ_MEMORY_BUILD_PATH="$MOZ_MEMORY_PATH/$MOZ_MEMORY_BUILD_DIR"
	MOZ_FALLIBLE_PATH="$MOZ_MEMORY_PATH/$MOZ_FALLIBLE_DIR"
	MOZ_JEMALLOC_PATH="$MOZ_MEMORY_PATH/$MOZ_JEMALLOC_DIR"
	MOZ_MOZALLOC_PATH="$MOZ_MEMORY_PATH/$MOZ_MOZALLOC_DIR"
	MOZ_MOZJEMALLOC_PATH="$MOZ_MEMORY_PATH/$MOZ_MOZJEMALLOC_DIR"
	MOZ_MEMORY_REPLACE_PATH="$MOZ_MEMORY_PATH/$MOZ_MEMORY_REPLACE_DIR"
		MOZ_MEMORY_REPLACE_MALLOC_PATH="$MOZ_MEMORY_REPLACE_PATH/$MOZ_MEMORY_REPLACE_DIR"
	
MOZ_MFBT_PATH="$MOZ_TOP_SRC_PATH/$MOZ_MFBT_DIR"

MOZ_INTL_PATH="$MOZ_TOP_SRC_PATH/$MOZ_INTL_DIR"
	MOZ_UCONV_PATH="$MOZ_INTL_PATH/$MOZ_UCONV_DIR"
	MOZ_UNICHARUTIL_PATH="$MOZ_INTL_PATH/$MOZ_UNICHARUTIL_DIR"
		MOZ_UNICHARUTIL_UTIL_PATH="$MOZ_UNICHARUTIL_PATH/$MOZ_UTIL_DIR"
			MOZ_UNICHARUTIL_UTIL_INTERNAL_PATH="$MOZ_UNICHARUTIL_UTIL_PATH/$MOZ_INTERNAL_DIR"

MOZ_IPC_PATH="$MOZ_TOP_SRC_PATH/$MOZ_IPC_DIR"

MOZ_XPCOM_PATH="$MOZ_TOP_SRC_PATH/$MOZ_XPCOM_DIR"
	MOZ_XPCOM_BASE_PATH="$MOZ_XPCOM_PATH/$MOZ_BASE_DIR"
	MOZ_XPCOM_THREADS_PATH="$MOZ_XPCOM_PATH/$MOZ_THREADS_DIR"
	MOZ_XPCOM_GLUE_PATH="$MOZ_XPCOM_PATH/$MOZ_GLUE_DIR/$MOZ_STANDALONE_DIR"
	MOZ_XPCOM_STRING_PATH="$MOZ_XPCOM_PATH/$MOZ_STRING_DIR"

MOZ_OBJ_GFX_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_GFX_DIR"
	MOZ_OBJ_2D_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_2D_DIR"
	MOZ_OBJ_CAIRO_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_CAIRO_DIR/$MOZ_CAIRO_DIR/$MOZ_SRC_DIR"
	MOZ_OBJ_CONFIG_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_CONFIG_DIR"
	MOZ_OBJ_GL_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_GL_DIR"
	MOZ_OBJ_LAYERS_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_LAYERS_DIR"
	MOZ_OBJ_PIXMAN_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_CAIRO_DIR/$MOZ_PIXMAN_DIR/$MOZ_SRC_DIR"
	MOZ_OBJ_SKIA_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_SKIA_DIR"
	MOZ_OBJ_GFX_SRC_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_SRC_DIR"
	MOZ_OBJ_THEBES_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_THEBES_DIR"

MOZ_OBJ_MEMORY_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_MEMORY_DIR"
	MOZ_OBJ_MEMORY_BUILD_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MEMORY_BUILD_DIR"
	MOZ_OBJ_FALLIBLE_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_FALLIBLE_DIR"
	MOZ_OBJ_JEMALLOC_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_JEMALLOC_DIR"
	MOZ_OBJ_MOZALLOC_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MOZALLOC_DIR"
	MOZ_OBJ_MOZJEMALLOC_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MOZJEMALLOC_DIR"
	MOZ_OBJ_MEMORY_REPLACE_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MEMORY_REPLACE_DIR"
		MOZ_OBJ_MEMORY_REPLACE_MALLOC_PATH="$MOZ_OBJ_MEMORY_REPLACE_PATH/$MOZ_MEMORY_REPLACE_DIR"

MOZ_OBJ_MFBT_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_MFBT_DIR"

MOZ_OBJ_INTL_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_INTL_DIR"
	MOZ_OBJ_UCONV_PATH="$MOZ_OBJ_INTL_PATH/$MOZ_UCONV_DIR"
	MOZ_OBJ_UNICHARUTIL_PATH="$MOZ_OBJ_INTL_PATH/$MOZ_UNICHARUTIL_DIR"
		MOZ_OBJ_UNICHARUTIL_UTIL_PATH="$MOZ_OBJ_UNICHARUTIL_PATH/$MOZ_UTIL_DIR"
			MOZ_OBJ_UNICHARUTIL_UTIL_INTERNAL_PATH="$MOZ_OBJ_UNICHARUTIL_UTIL_PATH/$MOZ_INTERNAL_DIR"

MOZ_OBJ_IPC_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_IPC_DIR"

MOZ_OBJ_XPCOM_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_XPCOM_DIR"
	MOZ_OBJ_XPCOM_BASE_PATH="$MOZ_OBJ_XPCOM_PATH/$MOZ_BASE_DIR"
	MOZ_OBJ_XPCOM_THREADS_PATH="$MOZ_OBJ_XPCOM_PATH/$MOZ_THREADS_DIR"
	MOZ_OBJ_XPCOM_GLUE_PATH="$MOZ_OBJ_XPCOM_PATH/$MOZ_GLUE_DIR/$MOZ_STANDALONE_DIR"
	MOZ_OBJ_XPCOM_STRING_PATH="$MOZ_OBJ_XPCOM_PATH/$MOZ_STRING_DIR"

#MOZ_FORCE_UNDEFINE=('-DMOZILLA_INTERNAL_API' '-DIMPL_LIBXUL')
#MOZ_FORCE_UNDEFINE=('-DIMPL_LIBXUL')

MOZ_CONFIG_UNDEFINE=('MOZ_CRASHREPORTER' 'MOZ_CRASHREPORTER_ENABLE_PERCENT' 'MOZ_LOGGING' 'MOZ_MEMORY' 'MOZ_ENABLE_PROFILER_SPS')
MOZ_ADDITIONAL_MK=('ipdlsrcs.mk')

function all(){
	configure
	prepare
	build
}

function configure {
	# Enter mozilla central and configure repo
	cdDuring privConfigure $MOZ_CENTRAL
}

function prepare {
	# Enter object folder in repo and manually prepare backend makefiles
	cdDuring privPrepare $MOZ_TOP_OBJ_PATH
}

function build {
	# gfx
	recursiveBackendToCMake "$MOZ_OBJ_GFX_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_GFX_PATH
	# memory/build
	backendToCMake "$MOZ_OBJ_MEMORY_BUILD_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MEMORY_BUILD_PATH
	# memory/jemalloc
	backendToCMake "$MOZ_OBJ_JEMALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_JEMALLOC_PATH
	# memory/fallible
	backendToCMake "$MOZ_OBJ_FALLIBLE_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_FALLIBLE_PATH
	# memory/mozalloc
	backendToCMake "$MOZ_OBJ_MOZALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MOZALLOC_PATH
	# memory/mozjealloc
	backendToCMake "$MOZ_OBJ_MOZJEMALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MOZJEMALLOC_PATH
	# memory/replace/replace
	backendToCMake "$MOZ_OBJ_MEMORY_REPLACE_MALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MEMORY_REPLACE_MALLOC_PATH
	# intl/uconv
	backendToCMake "$MOZ_OBJ_UCONV_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_UCONV_PATH
	# intl/unicharutil
	backendToCMake "$MOZ_OBJ_UNICHARUTIL_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_UNICHARUTIL_PATH
	# intl/unicharutil/util
	backendToCMake "$MOZ_OBJ_UNICHARUTIL_UTIL_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_UNICHARUTIL_UTIL_PATH
	# intl/unicharutil/util/internal
	backendToCMake "$MOZ_OBJ_UNICHARUTIL_UTIL_INTERNAL_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_UNICHARUTIL_UTIL_INTERNAL_PATH
	# mfbt
	backendToCMake "$MOZ_OBJ_MFBT_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MFBT_PATH
	# xpcom
	recursiveBackendToCMake "$MOZ_OBJ_XPCOM_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_XPCOM_PATH
	# config/external/nspr
	recursiveBackendToCMake "$MOZ_TOP_OBJ_PATH/config/external/nspr/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/config/external/nspr"
	# config/external/icu/common
	backendToCMake "$MOZ_TOP_OBJ_PATH/config/external/icu/common/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/config/external/icu/common"
	# config/external/icu/data
	backendToCMake "$MOZ_TOP_OBJ_PATH/config/external/icu/data/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/config/external/icu/data"
	# config/external/icu/stubdata
	backendToCMake "$MOZ_TOP_OBJ_PATH/config/external/icu/stubdata/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/config/external/icu/stubdata"
	# config/external/icu/i18n
	backendToCMake "$MOZ_TOP_OBJ_PATH/config/external/icu/i18n/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/config/external/icu/i18n"
	# modules/brotli
	backendToCMake "$MOZ_TOP_OBJ_PATH/modules/brotli/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/modules/brotli"
	# modules/libpref
	backendToCMake "$MOZ_TOP_OBJ_PATH/modules/libpref/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/modules/libpref"
	# modules/woff2
	backendToCMake "$MOZ_TOP_OBJ_PATH/modules/woff2/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/modules/woff2"
	# modules/zlib
	backendToCMake "$MOZ_TOP_OBJ_PATH/modules/zlib/src/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/modules/zlib/src"
	# mozglue/misc
	backendToCMake "$MOZ_TOP_OBJ_PATH/mozglue/misc/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/mozglue/misc"
	# hal
	backendToCMake "$MOZ_TOP_OBJ_PATH/hal/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/hal"
	# dom/base
	backendToCMake "$MOZ_TOP_OBJ_PATH/dom/base/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH "$MOZ_TOP_SRC_PATH/dom/base"
	# ipc
	recursiveBackendToCMake "$MOZ_OBJ_IPC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_IPC_PATH
}

function apply_shell_expansion() {
    declare file="$1"
    declare data=$(< "$file")
    declare delimiter="__apply_shell_expansion_delimiter__"
    declare command="cat <<$delimiter"$'\n'"$data"$'\n'"$delimiter"
    eval "$command"
}

# Private: 
function privConfigure {
	mach="$(pwd)/./mach"

	printf "%s\n" "$(apply_shell_expansion ../moz.config)" > .mozconfig

	$mach clobber
	echo "Configuring mozilla build..."
	$mach configure
}

function privPrepare {
	make recurse_pre-export
	make mozilla-config.h buildid.h source-repo.h
	
	echo "Undefining options..."
	for option in "${MOZ_CONFIG_UNDEFINE[@]}"
	do
		echo "$option disabled"
		echo "#undef $option" >> mozilla-config.h
	done
	
	make recurse_export
}

function backendToCMake {
	local backend=$1
	local topobjdir=$2
	local topsrcdir=$3
	local srcdir=$4
	echo "Parsing $backend... ($(backendLibraryName $backend))"
	generateBackendToCMake $backend $topobjdir $topsrcdir $srcdir > $(dirname $backend)"/CMakeLists.txt"
}


function recursiveBackendToCMake {
	local backend=$1
	local topobjdir=$2
	local topsrcdir=$3
	local srcdir=$4
	
	local dirs=($(backendDirs $backend))
	backendToCMake "$@"
	printf '%s\n' $(generateCMakeSubdirs dirs[@]) >> $(dirname $backend)"/CMakeLists.txt"
	
	for dir in "${dirs[@]}"
	do
		local newBackend="$(dirname $backend)/$dir/$(basename $backend)"
		local newSrcdir="$srcdir/$dir"
		recursiveBackendToCMake $newBackend $topobjdir $topsrcdir $newSrcdir
	done
}

# Generates a content for CMakeLists.txt from
# backend.mk located at path specified by first
# argument
function generateBackendToCMake {
	backend=$1
	topobjdir=$2
	topsrcdir=$3
	srcdir=$4

	local libName=$(backendLibraryName $backend)
	local defines=($(backendDefines $backend))
	local sources=($(backendSources $backend))
	local unifiedSources=($(backendUnifiedSources $backend))
	local localIncludes=($(backendLocalIncludes $backend $topobjdir $topsrcdir $srcdir))
	local customIncludes=($(backendCustomIncludes $backend 'dist/include'))
	local cxxFlags=($(backendCXXFlags $backend))

	#echo $libName
	#echo "${defines[@]}"
	#echo "${sources[@]}"
	#echo "${unifiedSources[@]}"
	#echo "${localIncludes[@]}"
	
	printf '%s\n' $(generateCMakeDefines defines[@])
	echo ""
	printf '%s\n' $(generateCMakeIncludes localIncludes[@])
	echo ""
	printf '%s\n' $(generateCMakeIncludes customIncludes[@] $topobjdir)
	echo ""
	echo $(generateCMakeSources sources[@] unifiedSources[@] $srcdir $(cmakeLibSources $libName))
	
	if [[ (${#sources[@]} -gt 0) || (${#unifiedSources[@]} -gt 0) ]]; then
		echo ""
		echo $(generateCMakeObjectLibrary $libName)
	fi

	echo ""
	echo $(generateCMakeCXXFlags cxxFlags[@] $libName)
	echo ""
	IFS_backup=$IFS
	IFS=$'\n'
	printf '%s\n' $(generateCMakeSourceFlags $backend sources[@])
	printf '%s\n' $(generateCMakeSourceFlags $backend unifiedSources[@])
	IFS=$IFS_backup
}

function generateCMakeSubdirs {
	declare -a dirs=("${!1}")
	
	local result=()
	
	for dir in "${dirs[@]}"
	do
		result+=('add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/'"$dir)")
	done

	printf '%s\n' "${result[@]}"
}

function generateCMakeDefines {
	declare -a defines=("${!1}")
	
	result=()
	for define in "${defines[@]}"
	do
		result+=("add_definitions($define)")
	done

	printf '%s\n' "${result[@]}"
}

function generateCMakeObjectLibrary {
	if [ $# -lt 1 ]; then
		return
	fi

	local libName=$1

	echo "add_library($libName OBJECT "'${'$(cmakeLibSources $libName)'})'
}

function generateCMakeIncludes {
	declare -a includes=("${!1}")
	relativePath=""

	if [ $# -gt 1 ]; then
		relativePath="/"$2
	fi

	result=()
	for include in "${includes[@]}"
	do
		result+=('include_directories(${PROJECT_SOURCE_DIR}'"$relativePath/$include)")
	done

	printf '%s\n' "${result[@]}"
}

function generateCMakeCXXFlags {
	declare -a flags=("${!1}")
	local libraryName=$2

	if [ $# -lt 2 ]; then
		return
	fi
	
	if [ ${#flags[@]} -gt 0 ]; then
		local escapedFlags="${flags[@]}"
		escapedFlags=${escapedFlags//\"/\\\"} 
		echo "set_target_properties ($libraryName PROPERTIES COMPILE_FLAGS "'"'$escapedFlags'")'
	fi
	
}

function generateCMakeSources {
	declare -a sources=("${!1}")
	declare -a unifiedSources=("${!2}")
	local relativePath=$3
	
	local result=()
	
	if [[ (${#sources[@]} -eq 0) && (${#unifiedSources[@]} -eq 0) ]]; then
		return
	fi
	
	var="src"
	if [ $# -gt 3 ]; then
		var=$4
	fi

	for source in "${sources[@]}"
	do
		result+=('${PROJECT_SOURCE_DIR}/'$relativePath"/"$source)
	done
	
	for source in "${unifiedSources[@]}"
	do
		result+=($source)
	done
	
	echo "set($var ${result[@]})"
}

function generateCMakeSourceFlags {
	local backend=$1
	declare -a sources=("${!2}")
	local flags=()
	
	for rawSource in "${sources[@]}"
	do
		local source=$(basename $rawSource)
		IFS_backup=$IFS
		IFS=$'\n'
		lines=($(cat "$backend" | grep  "^$source"'_FLAGS += '))
		IFS=$IFS_backup
		
		for line in "${lines[@]}"
		do	
			flag=${line#*=}
			flag=$(echo $flag | xargs)
			flags+=("set_source_files_properties($source PROPERTIES COMPILE_FLAGS "'"'$flag'")')
		done
	done
	IFS_backup=$IFS
	IFS=$'\n'
	printf '%s\n' "${flags[*]}"
	IFS=$IFS_backup
}

function cmakeLibSources {
	name='src'
	if [ $# -gt 0 ]; then
		name=$1"_src"
	fi
	echo $name
}

function backendLocalIncludes {
	backend=$1
	topobjdir=$2
	topsrcdir=$3
	srcdir=$4
	CURDIR=$(dirname $backend)
	
	includes=()
	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep "^LOCAL_INCLUDES += "))
	IFS=$IFS_backup
	
	for line in "${lines[@]}"
	do
		candidate=$(echo $line | cut -d "=" -f2)
		candidate=$(echo $candidate | xargs)
		if [[ "$candidate" =~ \I\$\([a-zA-Z_]+\) ]]; then
			candidate="${candidate/\(/{}"
			candidate="${candidate/\)/\}}"
			candidate="${candidate/\\/}"
			candidate="${candidate/-I/}"

			absolutePath=$(eval "echo $candidate")
			includes+=($absolutePath)
		else
			echo "Can not handle relative includes!" 1>&2
			exit 64
		fi
	done

	# Also include backend dir if not already included
	if [[ ! ( " ${includes[@]} " =~ " ${CURDIR} " ) ]]; then
		includes+=($CURDIR)
	fi
	# Also include src dir if not already included
	if [[ ! ( " ${includes[@]} " =~ " ${srcdir} " ) ]]; then
		includes+=($srcdir)
	fi
	# Also include top obj dir if not already included
	if [[ ! ( " ${includes[@]} " =~ " ${topobjdir} " ) ]]; then
		includes+=($topobjdir)
	fi
	
	echo "${includes[@]}"
	
}

function backendLibraryName {
	backend=$1
	
	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep "^LIBRARY_NAME := "))
	IFS=$IFS_backup
	
	name=${lines[0]}
	name=${name#*=}
	name=$(echo $name | xargs)
	echo $name
}

# Returns all defined symbols from backend file
function backendDefines {
	backend=$1
	defines=()

	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep "^DEFINES += "))
	IFS=$IFS_backup
	
	for line in "${lines[@]}"
	do
		definesString=${line#*=}
		tmpDefines=($(echo $definesString))
		for define in "${tmpDefines[@]}"
		do
			# Check if preprocess symbol is allowed
			if [[ ! ( " ${MOZ_FORCE_UNDEFINE[@]} " =~ " ${define} " ) ]]; then
				defines+=($define)
			fi
		done
	done
	
	echo "${defines[@]}"
}

function backendSources {
	backend=$1
	
	sources=()

	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep  "^CPPSRCS += \|^CSRCS += \|^ASFILES += \|^CMMSRCS += "))

	IFS=$IFS_backup
	
	for line in "${lines[@]}"
	do	
		sourcesString=${line#*=}
		sourcesString=$(echo $sourcesString | xargs)
		if [[ ! ("$sourcesString" =~ \$\([a-zA-Z_]+\) ) ]]; then
			tmpSources=($(echo $sourcesString))
			for source in "${tmpSources[@]}"
			do
				sources+=($source)
			done
		fi
	done

	echo "${sources[@]}"
}

function backendUnifiedSources {
	backend=$1
	sources=()

	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep "^UNIFIED_CPPSRCS := \|^UNIFIED_CSRCS := \|^CPPSRCS := \|^UNIFIED_CMMSRCS := "))
	IFS=$IFS_backup
	
	for line in "${lines[@]}"
	do
		sourcesString=${line#*=}
		tmpSources=($(echo $sourcesString))
		for source in "${tmpSources[@]}"
		do
			sources+=($source)
		done
	done

	if [ -z "$2" ]; then
	    local path=$(dirname $backend)
		for file in "${MOZ_ADDITIONAL_MK[@]}"
		do
			local tryBackend="$path/$file"
			if [ -f $tryBackend ]; then
				local additionals=$(backendUnifiedSources $tryBackend 1)
				for additional in "${additionals[@]}"
				do
					sources+=($additional)
				done
			fi
		done
	fi

	echo "${sources[@]}"
}

function backendCXXFlags {
	backend=$1
	flags=()

	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep  "^MOZBUILD_CXXFLAGS += \|^MOZBUILD_CFLAGS += \|^MOZBUILD_ASFLAGS += "))
	IFS=$IFS_backup
	
	for line in "${lines[@]}"
	do	
		flag=${line#*=}
		flag=$(echo $flag | xargs)
		flags+=($flag)
	done

	echo "${flags[@]}"
}

function backendCustomIncludes {
	backend=$1
	local dest="$2/"
	local includes=()
	
	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep  "^${dest//[\/]/_}"))
	IFS=$IFS_backup

	for line in "${lines[@]}"
	do
		local lineBefore=${line##*dist_include_}
		lineBefore=${lineBefore%% *}
		local mode=${lineBefore##*_}
		
		if [ "$mode" == "FILES" ]; then
			path=${lineBefore%%_FILES*}
			path=${path//[_]/\/}
			includes+=($dest$path)
			#local files=${line##*=}
			#files=($(echo $files | xargs))
			#for file in "${files[@]}"
			#do
			#	local include="$dest$path/$file"
			#	includes+=($include)
			#done
		fi
	done
	
	echo "${includes[@]}"
}

function backendDirs {
	backend=$1
	local dirs=()
	
	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep  "^DIRS := "))
	IFS=$IFS_backup
	
	for line in "${lines[@]}"
	do
		dirsString=${line#*=}
		tmpDirs=($(echo $dirsString))
		for dir in "${tmpDirs[@]}"
		do
			dirs+=($dir)
		done
	done

	echo "${dirs[@]}"
}

function compile {
	make
}

# Accept function as first parameter to be executed
# in a context of folder passed as second argument
function cdDuring() {
	echo "Entering ${2}..."
	current_folder=$(pwd)
	cd $2
	"$@"
	echo "Leaving ${2}"
	cd $current_folder
}

eval $OPERATION