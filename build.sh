#!/bin/bash

OPERATION='all'

if [ $# -gt 0 ]; then
	OPERATION=$1
fi

function all {
	node install/index.js
}

function help {
	echo "usage: [operation]

operation: all, help"
}

eval $OPERATION "${@:2}"