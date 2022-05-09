#! /bin/bash
# Usage: clean_build.sh

echo "cleaning build ..."

rm -rf out

make clean
if [ $? != 0 ]; then
	echo "clean build error!"
	exit 1
else
	echo "clean build done."
fi
