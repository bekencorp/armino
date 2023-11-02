#!/bin/bash

set -e
set -u

ARMINO_TARGET="build"
ARMINO_DIR=$1
PROJECT_DIR=$2
BUILD_DIR=$3
BUILD_TARGET=$4

ARMINO_TOOL=${ARMINO_DIR}/tools/build_tools/armino
BUILD_TARGET_PREFIX=${BUILD_TARGET:0:5}

need_build_properties_lib=0
need_build_soc=0
need_clean=0
need_doc=0

if [ "${BUILD_TARGET_PREFIX}" == "libbk" ]; then
	ARMINO_SOC=${BUILD_TARGET#lib}
	need_build_properties_lib=1
	BUILD_DIR=build/properties_libs/${ARMINO_SOC}_${PROJECT}
	PROJECT_DIR=projects/properties_libs
	echo "build armino ${ARMINO_SOC} properties libs only"
	export LIB_HASH="NULL"
elif [ "${BUILD_TARGET_PREFIX}" == "relbk" ]; then
	ARMINO_SOC=${BUILD_TARGET#rel}
	BUILD_DIR=build/${BUILD_DIR}/${ARMINO_SOC}
	need_build_properties_lib=0
	need_build_soc=1
	echo "build armino ${ARMINO_SOC} only"
	export LIB_HASH="NULL"
elif [ "${BUILD_TARGET_PREFIX}" == "clean" ]; then
	ARMINO_SOC=${BUILD_TARGET#clean}
	BUILD_DIR=build/${BUILD_DIR}/${ARMINO_SOC}
	need_clean=1
elif [ "${BUILD_TARGET_PREFIX}" == "docbk" ]; then
	ARMINO_SOC=${BUILD_TARGET#doc}
	BUILD_DIR=build/${BUILD_DIR}/${ARMINO_SOC}
	echo "build armino ${ARMINO_SOC} doc only"
	need_doc=1
else
	ARMINO_SOC=${BUILD_TARGET}
	BUILD_DIR=${BUILD_DIR}/${ARMINO_SOC}
	need_build_soc=1
	has_properties_lib_src=$(${ARMINO_DIR}/tools/build_tools/detect_internal_lib_src.py)
	if [ ${has_properties_lib_src} == "1" ]; then
		echo "build armino properties lib first, then ${ARMINO_SOC}"
		need_build_properties_lib=1
		export LIB_HASH="NULL"
	else
		need_build_properties_lib=0
		compute_hash_tool=${ARMINO_DIR}/tools/build_tools/compute_files_hash.py
		export LIB_HASH=$(python3 ${compute_hash_tool} ${ARMINO_DIR}/components/bk_libs/${ARMINO_SOC}_${PROJECT}/libs)
	fi
fi

PROPERTIES_LIB_BUILD_DIR=${ARMINO_DIR}/build/properties_libs/${ARMINO_SOC}_${PROJECT}
PROPERTIES_LIB_DIR=${PROPERTIES_PROJECT_DIR}

if [ "${need_clean}" == "1" ]; then
	echo "remove ${ARMINO_DIR}/components/bk_libs/${ARMINO_SOC}_${PROJECT}"
	rm -rf ${ARMINO_DIR}/components/bk_libs/${ARMINO_SOC}_${PROJECT}
	echo "remove ${ARMINO_DIR}/${BUILD_DIR}"
	rm -rf ${ARMINO_DIR}/${BUILD_DIR}
	echo "remove ${PROPERTIES_LIB_BUILD_DIR}"
	rm -rf ${PROPERTIES_LIB_BUILD_DIR}
	python ${ARMINO_DIR}/tools/build_tools/armino_doc.py clean ${ARMINO_SOC}
	exit 0
fi

if [ "${need_doc}" == "1" ]; then
	${ARMINO_TOOL} -B ./${BUILD_DIR} -P ./${PROJECT_DIR} doc ${ARMINO_SOC}
	exit 0
fi

if [ "${need_build_properties_lib}" == "1" ]; then
	echo "build properties lib for ${ARMINO_SOC}"
	rm -rf ${PROPERTIES_LIB_BUILD_DIR}/sdkconfig
	rm -rf ${ARMINO_DIR}/components/bk_libs/${ARMINO_SOC}_${PROJECT}
	${ARMINO_TOOL} -B ${PROPERTIES_LIB_BUILD_DIR} -P ${PROPERTIES_LIB_DIR} set-target ${ARMINO_SOC}
	${ARMINO_TOOL} -B ${PROPERTIES_LIB_BUILD_DIR} -P ${PROPERTIES_LIB_DIR} ${ARMINO_TARGET}
	${ARMINO_DIR}/tools/build_tools/copy_internal_libs.sh ${ARMINO_SOC} ${ARMINO_DIR} ${PROPERTIES_LIB_BUILD_DIR} ${PROJECT}
fi

if [ "${need_build_soc}" == "1" ]; then
	echo "build ${ARMINO_SOC}"
	rm -rf ${ARMINO_DIR}/${BUILD_DIR}/sdkconfig
	${ARMINO_TOOL} -B ${ARMINO_DIR}/${BUILD_DIR} -P ${ARMINO_DIR}/${PROJECT_DIR} set-target ${ARMINO_SOC}
	${ARMINO_TOOL} -B ${ARMINO_DIR}/${BUILD_DIR} -P ${ARMINO_DIR}/${PROJECT_DIR} ${ARMINO_TARGET}
	${ARMINO_DIR}/tools/build_tools/armino_as_lib.sh ${ARMINO_SOC} ${ARMINO_DIR} ${ARMINO_DIR}/${BUILD_DIR} ${PROJECT}
fi

