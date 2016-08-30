#!/bin/bash

options=("$@")

function install {
	if hash nodejs 2>/dev/null; then
		nodejs install/index.js "$@"
	else
		if hash node 2>/dev/null; then
			node install/index.js "$@"
		else
			echo "Error: Node.js not found"
		fi
	fi
}


install ${options[@]}
