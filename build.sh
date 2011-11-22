#!/bin/bash

# Builds a sub-project
# Example use:
# $ ./makesub tests/example --arduino

if [ $# -lt 1 ]; then
	FOLDER="all"
else
	FOLDER=$1
fi

if [ "$FOLDER" != "all" ]; then
	cd $FOLDER
fi

echo "Compiling $FOLDER"
/usr/local/bin/scons
