#!/bin/bash

init_bk_libs_dir()
{
	rm -rf ${s_bk_libs_dir}/${s_soc}
	mkdir -p ${s_bk_libs_dir}/${s_soc}
	mkdir -p ${s_bk_libs_dir}/${s_soc}/libs
	mkdir -p ${s_bk_libs_dir}/${s_soc}/config
}

copy_libs()
{
	echo "Copy armino properties libs ${s_armino_dir}/components/bk_libs"

	for dir in ${s_armino_build_dir}/armino/*
	do
		if [ -d ${dir} ] && [ -f ${dir}/*.a ]; then
			#echo "copy ${dir}/.a"
			cp -rf ${dir}/*.a ${s_bk_libs_dir}/${s_soc}/libs/
		fi
	done
}

copy_sdkconfig()
{
	cp ${s_armino_build_dir}/sdkconfig ${s_bk_libs_dir}/${s_soc}/config/
	cp ${s_armino_build_dir}/config/sdkconfig.h ${s_bk_libs_dir}/${s_soc}/config
}

s_soc=$1
s_armino_dir=$2
s_armino_build_dir=$3
s_bk_libs_dir="${s_armino_dir}/components/bk_libs"

init_bk_libs_dir
copy_libs
copy_sdkconfig 
