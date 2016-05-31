#!/bin/bash

DEFAULT_MOZ_CENTRAL='mozilla-central'
MOZ_CENTRAL=$DEFAULT_MOZ_CENTRAL
MOZ_CENTRAL_URL='https://hg.mozilla.org/mozilla-central/archive/tip'
MOZ_CENTRAL_ARCHIVE_TYPE='.tar.gz'
PATCHES_DIR='patches'
OPERATION='all'

if [ $# -gt 0 ]; then
	OPERATION=$1
fi

if [ $# -gt 1 ]; then
	MOZ_CENTRAL=$2
fi

MOZ_CENTRAL_ARCHIVE=$MOZ_CENTRAL$MOZ_CENTRAL_ARCHIVE_TYPE

function all {
	echo "Performing all operations..."
	download
	unpack
	patches
}

function download {
	echo "Downloading as ${MOZ_CENTRAL_ARCHIVE}..."
	wget https://hg.mozilla.org/mozilla-central/archive/tip.tar.gz -O ${MOZ_CENTRAL_ARCHIVE}
}

function unpack {
	echo "Unpacking... may take a while"
	rm -rf $MOZ_CENTRAL
	mkdir $MOZ_CENTRAL && tar -zxf $MOZ_CENTRAL_ARCHIVE -C $MOZ_CENTRAL --strip-components 1
}

function patches {
	echo "Applying patches..."
	for file in $PATCHES_DIR"/"*.patch; do
	    patch -p0 -d $DEFAULT_MOZ_CENTRAL < $file
	done
}

function help {
	echo "usage: [operation][unpacked name]

operation: all (download and unpack), download, unpack
unpacked name: name of folder after extraction ($DEFAULT_MOZ_CENTRAL is default)"
}

eval $OPERATION