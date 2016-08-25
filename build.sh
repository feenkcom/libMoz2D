#!/bin/bash

options=("$@")

function install {
	node install/index.js "$@"
}


install ${options[@]}