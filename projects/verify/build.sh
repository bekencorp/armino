#! /bin/bash

BUILD_TYPE=$1
DEPLOY_DIR=$2

VERIFY_DIR=$(pwd)
ARMINO_DIR=${VERIFY_DIR}/../..
INSTALL_DIR=${VERIFY_DIR}/installs
AES_TOOL=${ARMINO_DIR}/tools/env_tools/beken_packager/cmake_xts_aes
CRC_TOOL=${ARMINO_DIR}/tools/env_tools/beken_packager/cmake_encrypt_crc

cd ${ARMINO_DIR}
mkdir -p ${INSTALL_DIR}

if [ "$BUILD_TYPE" == "" ] || [ "$BUILD_TYPE" == "spe" ] || [ "$BUILD_TYPE" == "all" ]; then
	echo "Build SPE ..."
	make bk7236 PROJECT=verify/spe

	if [ -d ${ARMINO_DIR}/build/spe/bk7236/install/outputs ]; then
		cp ${ARMINO_DIR}/build/spe/bk7236/install/outputs/*.bin ${INSTALL_DIR}
		cp ${ARMINO_DIR}/build/spe/bk7236/install/outputs/*.elf ${INSTALL_DIR}
	fi
	cp ${ARMINO_DIR}/build/spe/bk7236/all.bin ${INSTALL_DIR}/all.bin
	cp ${ARMINO_DIR}/build/spe/bk7236/app.bin ${INSTALL_DIR}/app.bin
	cp ${ARMINO_DIR}/build/spe/bk7236/app.elf ${INSTALL_DIR}/app.elf
	cp ${ARMINO_DIR}/build/spe/bk7236/app.map ${INSTALL_DIR}/app.map
	cd ${INSTALL_DIR}
	${AES_TOOL} encrypt -infile ${INSTALL_DIR}/app.bin -keywords 73c7bf397f2ad6bf4e7403a7b965dc5ce0645df039c2d69c814ffb403183fb18 -outfile ${INSTALL_DIR}/app_raw_xts.bin -startaddress 0
	${CRC_TOOL} -crc ${INSTALL_DIR}/app_raw_xts.bin
	cd -
fi

if [ "$BUILD_TYPE" == "nspe" ] || [ "$BUILD_TYPE" == "all" ]; then
	echo "Build NSPE ..."
	make bk7236 PROJECT=verify/nspe
	cp ${ARMINO_DIR}/build/nspe/bk7236/app.bin ${INSTALL_DIR}/app_ns.bin
	cp ${ARMINO_DIR}/build/nspe/bk7236/app.elf ${INSTALL_DIR}/app_ns.elf
fi

cd ${INSTALL_DIR}

if [ "$BUILD_TYPE" == "win30" ]; then
	echo "Deploy binaries to /win30/$USER/bk7236/verify"
	DST_DIR=/win30/$USER/bk7236/verify/${DEPLOY_DIR}
	mkdir -p /win30/$USER/bk7236
	mkdir -p /win30/$USER/bk7236/verify
	mkdir -p /win30/$USER/bk7236/verify/${DEPLOY_DIR}
	cp -r ${INSTALL_DIR}/* ${DST_DIR}/
fi

if [ "$BUILD_TYPE" == "clean" ]; then
	echo "remove ${INSTALL_DIR}"
	rm -rf ${INSTALL_DIR}
	echo "remove ${ARMINO_DIR}/build"
	rm -rf ${ARMINO_DIR}/build
fi
