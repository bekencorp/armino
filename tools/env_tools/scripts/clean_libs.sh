#! /bin/bash
# Usage: clean_libs.sh

echo "cleaning libs ..."

rm -rf ./beken378/components/lib/*

make cleanlib
if [ $? != 0 ]; then
	echo "clean libs error!"
	exit 1
else
	echo "clean libs done."
fi
