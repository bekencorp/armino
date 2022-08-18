#! /bin/bash

# Clean up the sourcce files after libs are generated
# example:
# generate_beken_lib.sh bk7231u [SDK_path]

source ./tools/scripts/build_include.sh

if [ "$2" == "" ]; then
	BEKEN_SDK_DIR=./beken378
else
	BEKEN_SDK_DIR=$2
fi

LIB_FILE=${BEKEN_SDK_DIR}/components/lib/lib*.a

LIB_STORE=${BEKEN_SDK_DIR}/components/lib/lib${1}/

echo "making ${1} libs..."

SYS_CFG_FILE=${BEKEN_SDK_DIR}/components/bk_config/sys_config_${1}.h
if [ ! -f ${SYS_CFG_FILE} ]; then
	echo "${SYS_CFG_FILE}: no such file!"
	exit 1
fi
echo "backup ${SYS_CFG_FILE}"
cp -f ${SYS_CFG_FILE} ${SYS_CFG_FILE}.bak

case $1 in
	bk7231u)
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_4_2 1
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_5_X 0
		modify_config ${SYS_CFG_FILE} CONFIG_WIFI_SENSOR 1
		make_target="ip ble sensor"
		;;
	bk7231n)
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_4_2 0
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_5_X 1
		modify_config ${SYS_CFG_FILE} CONFIG_WIFI_SENSOR 1
		make_target="ip ble sensor"
		;;
	bk7251)
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_4_2 1
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_5_X 0
		modify_config ${SYS_CFG_FILE} CONFIG_WIFI_SENSOR 1
		modify_config ${SYS_CFG_FILE} CONFIG_USB 1
		make_target="ip ble usb sensor"
		;;
	bk7271)
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_4_2 0
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_5_X 0
		modify_config ${SYS_CFG_FILE} CONFIG_WIFI_SENSOR 1
		modify_config ${SYS_CFG_FILE} CONFIG_USB 1
		modify_config ${SYS_CFG_FILE} CONFIG_BT 1
		make_target="ip usb sensor bt"
		;;
	bk7236a)
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_5_X 1
		modify_config ${SYS_CFG_FILE} CONFIG_BLE_4_2 0
		modify_config ${SYS_CFG_FILE} CONFIG_WIFI_SENSOR 1
		make_target="ip ble sensor"
		;;
	*)
		echo "unsupported platform"
		exit 1
		;;
esac

echo "make clean/cleanlib..."
make clean > /dev/null
make cleanlib > /dev/null

./tools/scripts/generate_sys_config.sh ${1}
if [ $? != 0 ]; then
	echo "generate sys config fail"
	exit 1
fi

for sub in ${make_target}
do
	echo "making lib ${sub}..."
	make $sub -j8
	if [ $? != 0 ]; then
		echo "make lib $sub fail"
		exit 1
	fi
done

mkdir ${LIB_STORE}

mv -f ${LIB_FILE} ${LIB_STORE}

echo "restore ${SYS_CFG_FILE}"
rm -f ${SYS_CFG_FILE}
mv -f ${SYS_CFG_FILE}.bak ${SYS_CFG_FILE}
