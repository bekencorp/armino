#!/bin/bash

init_bk_libs_dir()
{
	rm -rf ${s_bk_libs_dir}/${s_soc}_${s_project}
	mkdir -p ${s_bk_libs_dir}/${s_soc}_${s_project}
	mkdir -p ${s_bk_libs_dir}/${s_soc}_${s_project}/libs
	mkdir -p ${s_bk_libs_dir}/${s_soc}_${s_project}/config
}

copy_libs()
{
	echo "Copy armino properties libs ${s_armino_dir}/components/bk_libs"

	for dir in ${s_armino_build_dir}/armino/*
	do
		if [ -d ${dir} ] && [ -f ${dir}/*.a ]; then
			#echo "copy ${dir}/.a"
			cp -rf ${dir}/*.a ${s_bk_libs_dir}/${s_soc}_${s_project}/libs/
		fi
	done
	rm ${s_bk_libs_dir}/${s_soc}_${s_project}/libs/libmain.a

	cp -rf ${s_armino_dir}/properties/modules/audio_codec/aac_decoder/libaac_decoder.a ${s_bk_libs_dir}/${s_soc}_${s_project}/libs/
}

copy_sdkconfig()
{
	cp ${s_armino_build_dir}/sdkconfig ${s_bk_libs_dir}/${s_soc}_${s_project}/config/
	cp ${s_armino_build_dir}/config/sdkconfig.h ${s_bk_libs_dir}/${s_soc}_${s_project}/config
}

s_soc=$1
s_armino_dir=$2
s_armino_build_dir=$3
s_project=$4
s_bk_libs_dir="${s_armino_dir}/components/bk_libs"

init_bk_libs_dir
copy_libs
copy_sdkconfig 
