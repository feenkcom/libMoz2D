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
MOZ_ROOT=$(pwd)

MOZ_TOP_SRC_PATH=$MOZ_CENTRAL
MOZ_TOP_OBJ_PATH=$MOZ_TOP_SRC_PATH"/"$MOZ_OBJ_DIR

MOZ_GFX_DIR='gfx'
	MOZ_2D_DIR='2d'
	MOZ_CAIRO_DIR='cairo'
	MOZ_PIXMAN_DIR='libpixman'
	MOZ_SKIA_DIR='skia'

MOZ_MEMORY_DIR='memory'
	MOZ_MEMORY_BUILD_DIR='build'
	MOZ_MEMORY_REPLACE_DIR='replace'
	MOZ_JEMALLOC_DIR='jemalloc'
	MOZ_MOZALLOC_DIR='mozalloc'
	MOZ_MOZJEMALLOC_DIR='mozjemalloc'
	
MOZ_XPCOM_DIR='xpcom'

MOZ_DISTDIR='dist'
MOZ_INCLUDEDIR='include'
MOZ_SRC_DIR='src'
MOZ_BASE_DIR='base'
MOZ_NSPRPUBDIR='nsprpub'
MOZ_PRDIR='pr'
MOZ_CONFIGDIR='config'
MOZ_EXTERNALDIR='external'
MOZ_NSPRDIR='nspr'
MOZ_TESTINGDIR='testing'
MOZ_GTESTDIR='gtest'

MOZ_BACKEND_FILE='backend.mk'

MOZ_GFX_PATH="$MOZ_TOP_SRC_PATH/$MOZ_GFX_DIR"
	MOZ_2D_PATH="$MOZ_GFX_PATH/$MOZ_2D_DIR"
	MOZ_CAIRO_PATH="$MOZ_GFX_PATH/$MOZ_CAIRO_DIR/$MOZ_CAIRO_DIR/$MOZ_SRC_DIR"
	MOZ_PIXMAN_PATH="$MOZ_GFX_PATH/$MOZ_CAIRO_DIR/$MOZ_PIXMAN_DIR/$MOZ_SRC_DIR"
	MOZ_SKIA_PATH="$MOZ_GFX_PATH/$MOZ_SKIA_DIR"

MOZ_MEMORY_PATH="$MOZ_TOP_SRC_PATH/$MOZ_MEMORY_DIR"
	MOZ_MEMORY_BUILD_PATH="$MOZ_MEMORY_PATH/$MOZ_MEMORY_BUILD_DIR"
	MOZ_JEMALLOC_PATH="$MOZ_MEMORY_PATH/$MOZ_JEMALLOC_DIR"
	MOZ_MOZALLOC_PATH="$MOZ_MEMORY_PATH/$MOZ_MOZALLOC_DIR"
	MOZ_MOZJEMALLOC_PATH="$MOZ_MEMORY_PATH/$MOZ_MOZJEMALLOC_DIR"
	MOZ_MEMORY_REPLACE_PATH="$MOZ_MEMORY_PATH/$MOZ_MEMORY_REPLACE_DIR"
		MOZ_MEMORY_REPLACE_MALLOC_PATH="$MOZ_MEMORY_REPLACE_PATH/$MOZ_MEMORY_REPLACE_DIR"
	
MOZ_XPCOM_PATH="$MOZ_TOP_SRC_PATH/$MOZ_XPCOM_DIR"
	MOZ_XPCOM_BASE_PATH="$MOZ_XPCOM_PATH/$MOZ_BASE_DIR"

MOZ_OBJ_GFX_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_GFX_DIR"
	MOZ_OBJ_2D_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_2D_DIR"
	MOZ_OBJ_CAIRO_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_CAIRO_DIR/$MOZ_CAIRO_DIR/$MOZ_SRC_DIR"
	MOZ_OBJ_PIXMAN_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_CAIRO_DIR/$MOZ_PIXMAN_DIR/$MOZ_SRC_DIR"
	MOZ_OBJ_SKIA_PATH="$MOZ_OBJ_GFX_PATH/$MOZ_SKIA_DIR"

MOZ_OBJ_MEMORY_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_MEMORY_DIR"
	MOZ_OBJ_MEMORY_BUILD_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MEMORY_BUILD_DIR"
	MOZ_OBJ_JEMALLOC_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_JEMALLOC_DIR"
	MOZ_OBJ_MOZALLOC_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MOZALLOC_DIR"
	MOZ_OBJ_MOZJEMALLOC_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MOZJEMALLOC_DIR"
	MOZ_OBJ_MEMORY_REPLACE_PATH="$MOZ_OBJ_MEMORY_PATH/$MOZ_MEMORY_REPLACE_DIR"
		MOZ_OBJ_MEMORY_REPLACE_MALLOC_PATH="$MOZ_OBJ_MEMORY_REPLACE_PATH/$MOZ_MEMORY_REPLACE_DIR"

MOZ_OBJ_XPCOM_PATH="$MOZ_TOP_OBJ_PATH/$MOZ_XPCOM_DIR"
	MOZ_OBJ_XPCOM_BASE_PATH="$MOZ_OBJ_XPCOM_PATH/$MOZ_BASE_DIR"

MOZ_FORCE_UNDEFINE=('-DMOZILLA_INTERNAL_API' '-DIMPL_LIBXUL')

function all(){
	configure
	prepare
	build
}

function configure {
	# Enter mozilla central an configure repo
	cdDuring privConfigure $MOZ_CENTRAL
}

function prepare {
	# Enter object folder in repo and manually prepare backend makefiles
	cdDuring privPrepare $MOZ_TOP_OBJ_PATH
}

function build {
	# gfx/2d
	backendToCMake "$MOZ_OBJ_2D_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_2D_PATH
	# gfx/cairo
	backendToCMake "$MOZ_OBJ_CAIRO_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_CAIRO_PATH
	# gfx/pixman
	backendToCMake "$MOZ_OBJ_PIXMAN_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_PIXMAN_PATH
	# gfx/skia
	backendToCMake "$MOZ_OBJ_SKIA_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_SKIA_PATH
	# memory/build
	backendToCMake "$MOZ_OBJ_MEMORY_BUILD_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MEMORY_BUILD_PATH
	# memory/jemalloc
	backendToCMake "$MOZ_OBJ_JEMALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_JEMALLOC_PATH
	# memory/mozalloc
	backendToCMake "$MOZ_OBJ_MOZALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MOZALLOC_PATH
	# memory/mozjealloc
	backendToCMake "$MOZ_OBJ_MOZJEMALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MOZJEMALLOC_PATH
	# memory/replace/replace
	backendToCMake "$MOZ_OBJ_MEMORY_REPLACE_MALLOC_PATH/$MOZ_BACKEND_FILE" $MOZ_TOP_OBJ_PATH $MOZ_TOP_SRC_PATH $MOZ_MEMORY_REPLACE_MALLOC_PATH
}

# Private: 
function privConfigure {
	mach="$(pwd)/./mach"

	cat > .mozconfig <<EOL

# Flags set for targeting x86.
export CROSS_COMPILE=1	

#CC+="clang -arch i386 -m32"
#CXX+="clang++ -arch i386 -m32"
#AR=ar

# Define where build files should go. This places them in the directory
# "obj-ff-dbg" under the current source directory
mk_add_options MOZ_OBJDIR=@TOPSRCDIR@/${MOZ_OBJ_DIR}

# -s makes builds quieter by default
# -j4 allows 4 tasks to run in parallel. Set the number to be the amount of
# cores in your machine. 4 is a good number.
mk_add_options MOZ_MAKE_FLAGS="-j4"

# Disable sandbox
ac_add_options --disable-sandbox

EOL

	$mach clobber
	echo "Configuring mozilla build..."
	$mach configure
}

function privPrepare {
	make recurse_pre-export
	make mozilla-config.h buildid.h source-repo.h
	# Logging causes more (build) problems than it solves
	echo "Logging disabled (#undef MOZ_LOGGING)!"
	echo "#undef MOZ_LOGGING" >> mozilla-config.h
	#echo "Disabling moz memory"
	echo "#undef MOZ_MEMORY" >> mozilla-config.h
	make recurse_export
}

function backendToCMake {
	local backend=$1
	local topobjdir=$2
	local topsrcdir=$3
	local srcdir=$4
	echo "Parsing $backend..."
	generateBackendToCMake $backend $topobjdir $topsrcdir $srcdir > $(dirname $backend)"/CMakeLists.txt"
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
	echo ""
	echo $(generateCMakeObjectLibrary $libName)
	echo ""
	echo $(generateCMakeCXXFlags cxxFlags[@] $libName)
	echo ""
	IFS_backup=$IFS
	IFS=$'\n'
	printf '%s\n' $(generateCMakeSourceFlags $backend sources[@])
	printf '%s\n' $(generateCMakeSourceFlags $backend unifiedSources[@])
	IFS=$IFS_backup
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
		echo "Need library name to generate code"
		exit 64
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
		echo "Need library name to generate code"
		exit 64
	fi
	
	if [ ${#flags[@]} -gt 0 ]; then
		echo "set_target_properties ($libraryName PROPERTIES COMPILE_FLAGS "'"'${flags[@]}'")'
	fi
	
}

function generateCMakeSources {
	declare -a sources=("${!1}")
	declare -a unifiedSources=("${!2}")
	local relativePath=$3
	
	local result=()
	
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
	lines=($(cat "$backend" | grep  "^CPPSRCS += \|^CSRCS += "))

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
	lines=($(cat "$backend" | grep "^UNIFIED_CPPSRCS := \|^UNIFIED_CSRCS := "))
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

	echo "${sources[@]}"
}

function backendCXXFlags {
	backend=$1
	flags=()

	IFS_backup=$IFS
	IFS=$'\n'
	lines=($(cat "$backend" | grep  "^MOZBUILD_CXXFLAGS += \|^MOZBUILD_CFLAGS +="))
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