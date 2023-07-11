#! /bin/bash

TARGET=$1
DEPLOY_DIR=$2

VERIFY_DIR=$(pwd)
ARMINO_DIR=${VERIFY_DIR}/../..
INSTALL_DIR=${VERIFY_DIR}/installs/${TARGET}
AES_TOOL=${ARMINO_DIR}/tools/env_tools/beken_packager/cmake_xts_aes
CRC_TOOL=${ARMINO_DIR}/tools/env_tools/beken_packager/cmake_encrypt_crc

cd ${ARMINO_DIR}
mkdir -p ${INSTALL_DIR}

if [ "$TARGET" == "bk7236" ] || [ "$TARGET" == "bk7258" ] || [ "$TARGET" == "all" ]; then
	echo "Build SPE ..."
	make $TARGET PROJECT=verify/spe

	if [ -d ${ARMINO_DIR}/build/spe/$TARGET/install/outputs ]; then
		cp ${ARMINO_DIR}/build/spe/$TARGET/install/outputs/*.bin ${INSTALL_DIR}
		cp ${ARMINO_DIR}/build/spe/$TARGET/install/outputs/*.elf ${INSTALL_DIR}
	fi
	cp ${ARMINO_DIR}/build/spe/$TARGET/all.bin ${INSTALL_DIR}/all.bin
	cp ${ARMINO_DIR}/build/spe/$TARGET/app.bin ${INSTALL_DIR}/app.bin
	cp ${ARMINO_DIR}/build/spe/$TARGET/app.elf ${INSTALL_DIR}/app.elf
	cp ${ARMINO_DIR}/build/spe/$TARGET/app.map ${INSTALL_DIR}/app.map
	cd ${INSTALL_DIR}
	${AES_TOOL} encrypt -infile ${INSTALL_DIR}/app.bin -keywords 73c7bf397f2ad6bf4e7403a7b965dc5ce0645df039c2d69c814ffb403183fb18 -outfile ${INSTALL_DIR}/app_raw_xts.bin -startaddress 0
	${CRC_TOOL} -crc ${INSTALL_DIR}/app_raw_xts.bin
	cd -
fi

cd ${INSTALL_DIR}

if [ "$DEPLOY_DIR" == "win30" ]; then
	echo "Deploy binaries to /win30/$USER/$TARGET/verify"
	DST_DIR=/win30/$USER/$TARGET/verify/
	mkdir -p /win30/$USER/$TARGET
	mkdir -p /win30/$USER/$TARGET/verify
	mkdir -p /win30/$USER/$TARGET/verify/
	cp -r ${INSTALL_DIR}/* ${DST_DIR}/
fi

if [ "$TARGET" == "clean" ]; then
	echo "remove ${INSTALL_DIR}"
	rm -rf ${INSTALL_DIR}
	echo "remove ${ARMINO_DIR}/build"
	rm -rf ${ARMINO_DIR}/build
fi
