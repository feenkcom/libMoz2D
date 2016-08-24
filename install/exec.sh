#!/bin/bash

# Fixes python's locale.Error: unsupported locale setting
export LC_ALL=C

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PATH=$PATH:${DIR}

COMMAND=""
DIRECTORY="."

if [ $# -gt 0 ]; then
	COMMAND=$1
	if [ $# -gt 1 ]; then
		DIRECTORY=$2
	fi
else
	echo "Command to execute is not specified!" 1>&2
	exit 2
fi

if [ ! -d ${DIRECTORY} ]; then
  echo "$DIRECTORY does not exist"
  exit 3
fi

set -e

cd ${DIRECTORY}
eval ${COMMAND}