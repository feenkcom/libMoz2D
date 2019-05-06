#!/bin/bash

mkdir -p mozilla-build
curl https://ftp.mozilla.org/pub/mozilla/libraries/win32/MozillaBuildSetup-2.2.0.exe --output mozilla-build/MozillaBuildSetup-2.2.0.exe
7z x mozilla-build/MozillaBuildSetup-2.2.0.exe -omozilla-build

./mozilla-build/start-shell-msvc2015-x64.bat