#!/bin/bash

OPERATION='all'

if [ $# -gt 0 ]; then
	OPERATION=$1
fi

function all {
	echo "Performing all operations..."
	sh script-mozilla-get.sh "$@"
	sh script-mozilla-prepare.sh "$@"
	build
}

function download {
	sh script-mozilla-get.sh download "$@"
}

function unpack {
	sh script-mozilla-get.sh unpack "$@"
}

function patch {
	sh script-mozilla-get.sh patches "$@"
}

function configure {
	sh script-mozilla-prepare.sh configure "$@"
}

function prepare {
	sh script-mozilla-prepare.sh prepare "$@"
}

function export {
	sh script-mozilla-prepare.sh build "$@"
}

function build {
	mkdir -p build
	cd build
	cmake ..
	make
	cd ..
}

function help {
	echo "usage: [operation][arguments]

operation: get (download, unpack, patches), prepare (configure, prepare, build cmakes)
arguments: sub-command arguments"
}

eval $OPERATION "${@:2}"