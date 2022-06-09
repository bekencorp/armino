#!/usr/bin/env bash
#******************************************************************************
#  * @attention
#  *
#  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
#  * All rights reserved.</center></h2>
#  * <h2><center>&copy; Copyright (c) 2021 Nordic Semiconductor ASA.
#  * </center></h2>
#  *
#  *
#  * This software component is licensed by ST under BSD 3-Clause license,
#  * the "License"; You may not use this file except in compliance with the
#  * License. You may obtain a copy of the License at:
#  *                        opensource.org/licenses/BSD-3-Clause
#  *
#  ******************************************************************************
# arg1 is optional, it fixes compiler full path if present
# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
projectdir=`dirname $SCRIPT`
source $projectdir/preprocess.sh
# Compiler full name & path is passed as argument
compiler_full_path=
if [ $# -eq 1 ]; then
    compiler_full_path=$1
fi
# the file to preprocess is generated and present outside of install dir
bl2_file_to_preprocess=$projectdir/image_macros_to_preprocess_bl2.c
preprocess_bl2_file=$projectdir/image_macros_preprocessed_bl2.c
regressionsh=$projectdir/regression.sh
updatesh=$projectdir/TFM_UPDATE.sh
bin2hexsh=$projectdir/TFM_BIN2HEX.sh
basedir=$projectdir
echo preprocess bl2 file
preprocess $projectdir $bl2_file_to_preprocess $preprocess_bl2_file $compiler_full_path
stm_tool=$basedir"/scripts/stm_tool.py"
#determine/check python version command
cmd="python3"
$cmd --version  &> /dev/null
ret=$?
if [ $ret != 0 ]; then
  cmd="python"
  $cmd --version  &> /dev/null
  ret=$?
  if [ $ret != 0 ]; then
    echo "This script requires python 3.0 or greater"
    exit 1
  fi
  ver=$(python -V 2>&1 | sed 's/.* \([0-9]\).\([0-9]\).*/\1\2/')
  if [ "$ver" -lt "30" ]; then
    echo "This script requires python 3.0 or greater"
    exit 1
  fi
fi

command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b secbootadd0 -m  RE_BL2_BOOT_ADDRESS  -d 0x80  -s 0 "$regressionsh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi

command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b boot -m  RE_BL2_BIN_ADDRESS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi

command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi

echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot2 -m  RE_IMAGE_FLASH_SECURE_UPDATE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot3 -m  RE_IMAGE_FLASH_NON_SECURE_UPDATE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b scratch -m  RE_IMAGE_FLASH_SCRATCH -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b nvcounter -m  RE_IMAGE_FLASH_NV_COUNTERS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b nvmcnt -m  RE_BL2_NVMCNT_ADDRESS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b prov -m  RE_BL2_PROVISION_ADDRESS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b sst -m  RE_IMAGE_FLASH_NV_PS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b its -m  RE_IMAGE_FLASH_NV_ITS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b unused -m  RE_IMAGE_FLASH_UNUSED -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b boot -m  RE_BL2_BIN_ADDRESS -s 0 "$bin2hexsh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi


command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE -s 0 "$bin2hexsh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi

echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE -s 0 "$bin2hexsh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
