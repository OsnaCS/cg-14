#!/bin/bash
TARGET="$1"

cd lumina
if [ "$TARGET" == "r" ]; then
	echo "==== building Lumina in release mode ===="
	make clean
	make RELEASE=1 -j4
else
	echo "==== building Lumina in debug mode ===="
	make clean
  make ASAN=1 -j4
fi
