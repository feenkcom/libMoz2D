#!/bin/bash

fold_start() {
  echo -e "travis_fold:start:$1\033[33;1m$2\033[0m"
}

fold_end() {
  echo -e "\ntravis_fold:end:$1\r"
}

fold_start moz.1 "Installing MozillaBuild"
mkdir -p mozilla-build

curl https://ftp.mozilla.org/pub/mozilla/libraries/win32/MozillaBuildSetup-2.2.0.exe --output mozilla-build/MozillaBuildSetup-2.2.0.exe
7z x mozilla-build/MozillaBuildSetup-2.2.0.exe -omozilla-build

curl https://nodejs.org/dist/v6.7.0/node-v6.7.0-x64.msi --output mozilla-build/node-v6.7.0-x64.msi
msiexec /i mozilla-build/node-v6.7.0-x64.msi /qn /log mozilla-build/nodejs-install.log

fold_end moz.1

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