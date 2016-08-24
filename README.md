# Moz2D
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

Node.js:<br>
`sudo apt-get install nodejs`

Yasm:<br>
`sudo apt-get install yasm`

Autoconf2.13:<br>
`sudo apt-get install autoconf2.13`

libgtk2.0-dev:<br>
`sudo apt-get install libgtk2.0-dev`

libgtk-3-dev:<br>
`sudo apt-get install libgtk-3-dev`

libgconf2-dev:<br>
`sudo apt-get install libgconf2-dev`

libglib2.0-dev:<br>
`sudo apt-get install libglib2.0-dev`

Alsa:<br>
`sudo apt-get install libasound2-dev`

#### 32bit

gcc-multilib:<br>
`sudo apt-get install gcc-multilib`

g++-multilib:<br>
`sudo apt-get install g++-multilib`

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