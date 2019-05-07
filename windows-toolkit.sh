#!/bin/bash

fold_start() {
  echo -e "travis_fold:start:$1\033[33;1m$2\033[0m"
}

fold_end() {
  echo -e "\ntravis_fold:end:$1\r"
}

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
export PATH=$PATH:/c/Program\ Files/7-Zip

fold_start moz.1 "Installing MozillaBuild"
mkdir -p mozilla-build

# MozillaBuild
if [ ! -f "mozilla-build/toolkit-install.log" ]; then
	curl https://ftp.mozilla.org/pub/mozilla/libraries/win32/MozillaBuildSetup-2.2.0.exe --output mozilla-build/MozillaBuildSetup-2.2.0.exe
	7z x mozilla-build/MozillaBuildSetup-2.2.0.exe -omozilla-build
	echo "Success" > mozilla-build/toolkit-install.log
fi

echo "if [ -f ~/.bashrc ]; then . ~/.bashrc; fi" > .bash_profile

echo '
if [[ \"$STARTUP_DIR\" != \"$PWD\" ]]; then
    cd $STARTUP_DIR
fi
export PATH=$PATH:/c/Program\ Files/nodejs
export PATH=$PATH:/c/Program\ Files/CMake/bin
export PATH=~/mozilla-build/bin:$PATH
unset CXX
unset CC
unset TRAVIS_COMPILER
unset CXX_FOR_BUILD
unset CC_FOR_BUILD
env
./build.sh
exit' > .bashrc

fold_end moz.1

export STARTUP_DIR=$(pwd)
export HOME=$(pwd)

fold_start moz.2 "Building Moz2D"
./mozilla-build/start-shell-msvc2015-x64.bat
fold_end moz.2