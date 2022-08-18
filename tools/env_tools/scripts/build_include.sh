#! /bin/bash

modify_config()
{
	sed -i 's/\(\s*#define\s*\)\<'$2'\>\(\s*\)\([0-9]*\)/\1'$2'\2'$3'/g' $1
}

SUPPORTED_BUILD_PLATFORMS="bk7231n bk7231u bk7251 bk7271 bk7236 bk7236a"

validate_platform()
{
	for PLATFORM in ${SUPPORTED_BUILD_PLATFORMS}
	do
		if [ $1 == $PLATFORM ]; then
			return 0
		fi
	done

	echo "Unknown platform $1!"
	return 1
}

backup_revision_file()
{
	echo "backup revision files"
	mv -f $1 ${1}.bak
	mv -f $2 ${2}.bak
}

restore_revision_file()
{
	echo "restore revision files"
	rm -f $1
	rm -f $2
	mv -f ${1}.bak $1
	mv -f ${2}.bak $2
}

gather_out_files()
{
	PLATFORM=$1
	OUT_FOLDER=$2

	if [ ! -f ./out/all_2M.1220.bin ]; then
		echo "Not found firmware all_2M.1220.bin"
		return 1
	fi

	if [ ! -d $OUT_FOLDER ]; then
		mkdir $OUT_FOLDER
	fi

	cp -f ./out/*.bin $OUT_FOLDER
	cp -f ./out/*.map $OUT_FOLDER
	cp -f ./out/*.elf $OUT_FOLDER

	if [ "$(ls -A $OUT_FOLDER)" == "" ]; then
		echo "Gather out files error!"
		return 1
	else
		echo "Gather out files done"
		return 0
	fi
}
