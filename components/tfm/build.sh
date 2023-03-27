export PATH=/opt/gcc-arm-none-eabi-9-2020-q2-update/bin:$PATH
NINJA_BUILD=1
BUILD_DIR=cmake_build

current_dir=$(pwd)
armino_path=${current_dir}/../../

cd tfm
if [ "$NINJA_BUILD" == "1" ]; then
    #cmake -G Ninja -S . -B ../$BUILD_DIR -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Release -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER -DARMINO_SDK_PATH=${armino_path} -DFPGA=ON -DMCUBOOT_LOG_LEVEL=DEBUG -DOTP_NV_COUNTERS_RAM_EMULATION=ON -DPLATFORM_DEFAULT_OTP_WRITEABLE=ON
    cmake -G Ninja -S . -B ../$BUILD_DIR -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Release -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER -DARMINO_SDK_PATH=${armino_path} -DFPGA=ON -DOTP_NV_COUNTERS_RAM_EMULATION=ON -DPLATFORM_DEFAULT_OTP_WRITEABLE=ON -DMCUBOOT_LOG_LEVEL=DEBUG
    cd ../$BUILD_DIR
    ninja -- install
else
    cmake -S . -B ../$BUILD_DIR -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Release -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER -DARMINO_SDK_PATH=${armino_path} -DFPGA=ON 
    cmake --build ../$BUILD_DIR -- install -j8
fi

cd ..
if [ ! -d build ]; then
	git clone "ssh://ming.liu@192.168.0.46:29418/wifi/bk7236/build"
	cd build
	git clone "ssh://ming.liu@192.168.0.46:29418/iot/wcn/tools/imgtools"
fi

${current_dir}/build/fpga_build.sh $current_dir $1 $2


