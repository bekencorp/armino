#!/bin/bash

init_dir()
{
	rm -rf ${s_armino_lib_dir}/${s_soc}
	mkdir -p ${s_armino_lib_dir}/
	mkdir -p ${s_armino_lib_dir}/${s_soc}
	mkdir -p ${s_armino_lib_dir}/${s_soc}/config
	mkdir -p ${s_armino_lib_dir}/${s_soc}/libs
}

generate_armino_as_lib()
{
	# copy properties libs and sdkconfig.h
	cp -rf ${s_bk_libs_dir}/${s_soc}_${s_project}/libs/* ${s_armino_lib_dir}/${s_soc}/libs/
	cp -rf ${s_bk_libs_dir}/${s_soc}_${s_project}/config/sdkconfig.h ${s_armino_lib_dir}/${s_soc}/config/sdkconfig.h.properties

	# copy matter libs
	if [ -d ${s_matter_lib_dir} ]; then
		cp -rf ${s_matter_lib_dir}/*.a ${s_armino_lib_dir}/${s_soc}/libs/
	fi

	# copy all other armino libs
	for dir in ${s_armino_build_dir}/armino/*
	do
		if [ -d ${dir} ] && [ -f ${dir}/*.a ]; then
			cp -rf ${dir}/*.a ${s_armino_lib_dir}/${s_soc}/libs/
		fi
	done
	cp ${s_armino_build_dir}/config/sdkconfig.h ${s_armino_lib_dir}/${s_soc}/config/

	# copy global headers
	cp -rf ${s_armino_dir}/include ${s_armino_lib_dir}

	echo "Found all libs in ${s_armino_lib_dir}"
}

s_soc=$1
s_armino_dir=$2
s_armino_build_dir=$3
s_project=$4
s_bk_libs_dir="${s_armino_dir}/components/bk_libs"
s_matter_lib_dir=${s_armino_build_dir}/armino/matter_build/out/${s_soc}/lib
s_armino_lib_dir=${s_armino_build_dir}/armino_as_lib

init_dir
generate_armino_as_lib
