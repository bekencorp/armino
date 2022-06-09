#!/usr/bin/env bash -
#******************************************************************************
#  * @attention
#  *
#  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
#  * All rights reserved.</center></h2>
#  *
#  * This software component is licensed by ST under BSD 3-Clause license,
#  * the "License"; You may not use this file except in compliance with the
#  * License. You may obtain a copy of the License at:
#  *                        opensource.org/licenses/BSD-3-Clause
#  *
#  ******************************************************************************

echo "TFM BIN2HEX started"
disk=d
if [ $# -eq 1 ]; then
disk="$1"
fi
disk_option="/"$disk

# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
SCRIPTPATH=`dirname $SCRIPT`
BINPATH="$SCRIPTPATH/bin"


echo "Write TFM_Appli Secure"
# part ot be updated according to flash_layout.h
slot0=
slot1=
boot=
unused=
#since drag an drop is not supporting flash secure alias, address is conververted to non secure flash alias
let image_s=$slot0-0x4000000
image_s=`printf 0x%x $image_s`
let image_ns=$slot1-0x4000000
image_ns=`printf 0x%x $image_ns`
let bl2=$boot-0x4000000
bl2=`printf 0x%x $bl2`
printf "disk "$disk_option", hex addresses: "$bl2", "$image_s", "$image_ns
python scripts/bin2hex.py --offset=$image_s $BINPATH/tfm_s_signed.bin  $BINPATH/tfm_s_signed.hex
python scripts/bin2hex.py --offset=$image_ns $BINPATH/tfm_ns_signed.bin  $BINPATH/tfm_ns_signed.hex
python scripts/bin2hex.py --offset=$bl2 $BINPATH/bl2.bin $BINPATH/bl2.hex
if [ ! -f $disk_option/DETAILS.TXT ]; then
    echo "disk not present"
    exit 1
fi
# cleaning 1st bank is ok for now.
echo "Cleaning 1st bank"
#create a file with 0 to clean up internal 1st bank : NVM, ITS and PS area
# must be clean
dd if=/dev/zero of=$BINPATH/clean.bin bs=1K count=256
python scripts/bin2hex.py --offset=0x8000000 $BINPATH/clean.bin $BINPATH/clean.hex
cp $BINPATH/clean.hex $disk_option
while [  -f $disk_option/clean.hex ];
do
    sleep 0.1
done
sleep 5
while [ ! -f $disk_option/DETAILS.TXT ]
do
    sleep 0.1
done
if [ -f $disk_option/failed.txt ]; then
    echo " tfm_s_signed.hex not written"
    cat $disk-option/failed.txt
    exit 1
fi
#write secure image
echo "Write Secure Image"
cp $BINPATH/tfm_s_signed.hex $disk_option
while [  -f $disk_option/tfm_s_signed.hex ];
do
    sleep 0.1
done
sleep 5
while [ ! -f $disk_option/DETAILS.TXT ]
do
    sleep 0.1
done
if [ -f $disk_option/failed.txt ]; then
    echo " tfm_s_signed.hex not written"
    cat $disk-option/failed.txt
    exit 1
fi
#write non secure image
echo "Write non secure image"
cp $BINPATH/tfm_ns_signed.hex $disk_option
while [  -f $disk_option/tfm_ns_signed.hex ];
do
    sleep 0.1
done
sleep 5
while [ ! -f $disk_option/DETAILS.TXT ]
do
    sleep 0.1
done
if [ -f $disk_option/failed.txt ]; then
    echo "tfm_ns_signed.hex not written"
    cat $disk_option/failed.txt
    exit 1
fi
#write bl2 image
echo "Write bl2 Image"
cp $BINPATH/bl2.hex $disk_option
while [  -f $disk_option/tfm_ns_signed.hex ];
do
    sleep 0.1
done
sleep 5
while [ ! -f $disk_option/DETAILS.TXT ]
do
    sleep 0.1
done
if [ -f $disk_option/failed.txt ]; then
    echo "bl2.hex not written"
    cat $disk_option/failed.txt
    exit 1
fi
