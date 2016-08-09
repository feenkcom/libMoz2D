# Moz2D
Scripts to compile `gfx` part of Mozilla Firefox as single shared library

Compile shared library (32 bit):

`./build.sh`

Binary will be in `build/` folder

## Dependencies
`node` `wget` `cmake >= 3.0` `make` `autoconf 2.13` `patch` `gcc-multilib >= 5.3`

### ArchLinux
Node.js:<br>
`yaourt -S nodejs`

Might require latest _icu_ for Node.js:<br>
`yaourt -S icu`

Autoconf2.13:<br>
`yaourt -S autoconf2.13`

GCC-Multilib:<br>
`yaourt -S gcc-multilib`

