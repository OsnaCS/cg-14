#!/bin/bash

TARGET="$1"

clear && clear
if [ "$TARGET" == "d" ]; then
	echo "==== building CraftGame in debug mode ===="
	make ASAN=1 -j4 && echo "==== running CraftGame ====" && ./app
else
	echo "==== building CraftGame in release mode ===="
	make RELEASE=1 -j4 && echo "==== running CraftGame ====" && ./app
fi
