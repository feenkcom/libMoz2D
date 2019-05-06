#!/bin/bash

mkdir -p mozilla-build

curl https://ftp.mozilla.org/pub/mozilla/libraries/win32/MozillaBuildSetup-2.2.0.exe --output mozilla-build/MozillaBuildSetup-2.2.0.exe
#wget -O mozilla-build/MozillaBuildSetup-2.2.0.exe https://ftp.mozilla.org/pub/mozilla/libraries/win32/MozillaBuildSetup-2.2.0.exe

7z x mozilla-build/MozillaBuildSetup-2.2.0.exe -omozilla-build

echo "if [ -f ~/.bashrc ]; then . ~/.bashrc; fi" > .bash_profile

echo '
if [[ \"$STARTUP_DIR\" != \"$PWD\" ]]; then
    cd $STARTUP_DIR
fi
export PATH=$PATH:/c/Program\ Files/nodejs
export PATH=$PATH:/c/Program\ Files/CMake/bin
export PATH=~/mozilla-build/bin:$PATH
./build.sh
exit' > .bashrc

export STARTUP_DIR=$(pwd)
export HOME=$(pwd)

./mozilla-build/start-shell-msvc2015-x64.bat