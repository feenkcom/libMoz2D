#!/bin/bash

mkdir -p mozilla-build
ls -la

curl https://ftp.mozilla.org/pub/mozilla/libraries/win32/MozillaBuildSetup-2.2.0.exe --output mozilla-build/MozillaBuildSetup-2.2.0.exe
#wget -O mozilla-build/MozillaBuildSetup-2.2.0.exe https://ftp.mozilla.org/pub/mozilla/libraries/win32/MozillaBuildSetup-2.2.0.exe

7z x mozilla-build/MozillaBuildSetup-2.2.0.exe -omozilla-build

echo "if [ -f ~/.bashrc ]; then . ~/.bashrc; fi" >> .bash_profile

cat >> .bashrc <<EOL
if [[ \`"$STARTUP_DIR\`" != \`"$PWD\`" ]]; then
    cd $STARTUP_DIR
fi
export PATH=`$PATH:/c/Program\ Files/nodejs
export PATH=`$PATH:/c/Program\ Files/CMake/bin
export PATH=~/$MOZILLA_BUILD/bin:`$PATH
./build.sh
EOL

ls -la

./mozilla-build/start-shell-msvc2015-x64.bat