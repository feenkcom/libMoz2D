# Moz2D
Scripts to compile `gfx` part of Mozilla Firefox as single shared library

Compile shared library:

`./build.sh`

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

## Build process can be manually executed step-by-step

1. Download mozilla sources:</br>
`./build.sh download`
2. Extract sources:</br>
`./build.sh unpack`
3. Apply patches:</br>
`./build.sh patch`
4. Run mozilla configure scripts:</br>
`./build.sh configure`
5. Prepare mozilla makefiles (backend.mk):</br>
`./build.sh prepare`
6. Parse and convert mozilla's backend.mk files to CMakeLists.txt:</br>
`./build.sh export`
7. Compile single shared library with C bindings:</br>
`./build.sh build`
