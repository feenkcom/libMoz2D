# Moz2D [![Build Status](https://travis-ci.org/syrel/Moz2D.svg?branch=master)](https://travis-ci.org/syrel/Moz2D)
Scripts to compile `gfx` part of Mozilla Firefox as single shared library

Compile shared library (32 bit):

`./build.sh`

Binary will be in `build/` folder

## Dependencies
Tools:<br>
`git >= 1.7.12` `node >= v0.11.12` `python` `cmake >= 3.0` `make` `autoconf 2.13` `gcc-multilib >= 5.3`<br>
Utilities:<br>
`wget` `patch` `which` `tar`

### Ubuntu / Travis

The following packages are required to build 64bit plugin

	sudo apt-get install yasm autoconf2.13 libgtk2.0-dev libgtk-3-dev libgconf2-dev libglib2.0-dev libasound2-dev libpulse-dev

#### 32bit

32bit build requires a few additional packages

	sudo apt-get install gcc-multilib g++-multilib

### ArchLinux
Node.js:<br>
`yaourt -S nodejs`

Might require latest _icu_ for Node.js:<br>
`yaourt -S icu`

Autoconf2.13:<br>
`yaourt -S autoconf2.13`

GCC-Multilib:<br>
`yaourt -S gcc-multilib`

### Windows

First [install MingGW](https://sourceforge.net/projects/mingw/files/) and follow "[Environment Settings](http://www.mingw.org/wiki/Getting_Started#toc7)" instruction to add MinGW to PATH environment variable. If installed with default settings a path should be `C:\MinGW\bin`<br>
Additional packages are required, install them by executing:<br>
`mingw-get install msys-unzip msys-wget msys-zip msys-tar msys-patch`<br>
MSYS also needs to be added to the PATH variable, by default it is `C:\MinGW\msys\1.0\bin;C:\MinGW\msys\1.0\local\bin`

Installation scripts require latest version of [Node.js](https://nodejs.org/en/).