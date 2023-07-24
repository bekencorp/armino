#!/bin/bash

init_bk_libs_dir()
{
	rm -rf ${s_bk_libs_dir}/${s_soc}_${s_project}
	mkdir -p ${s_bk_libs_dir}/${s_soc}_${s_project}
	mkdir -p ${s_bk_libs_dir}/${s_soc}_${s_project}/libs
	mkdir -p ${s_bk_libs_dir}/${s_soc}_${s_project}/config
	mkdir -p ${s_bk_libs_dir}/${s_soc}_${s_project}/hash
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
	#find ${s_bk_libs_dir}/${s_soc}_${s_project}/libs -name *.a | xargs md5sum > ${s_bk_libs_dir}/${s_soc}_${s_project}/hash/libs_hash.txt
	compute_hash_tool=${s_armino_dir}/tools/build_tools/compute_files_hash.py
	compute_hash_src_dir=${s_bk_libs_dir}/${s_soc}_${s_project}/libs
	compute_hash_dest_file=${s_bk_libs_dir}/${s_soc}_${s_project}/hash/libs_hash.txt
	python3 ${compute_hash_tool} ${compute_hash_src_dir} > ${compute_hash_dest_file}
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
