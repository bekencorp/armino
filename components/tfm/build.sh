export PATH=/opt/gcc-arm-none-eabi-9-2020-q2-update/bin:$PATH
NINJA_BUILD=1
BUILD_DIR=cmake_build

cd tfm
current_dir=$(pwd)
armino_path=${current_dir}/../../../

if [ "$NINJA_BUILD" == "1" ]; then
    cmake -G Ninja -S . -B ../$BUILD_DIR -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Release -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER -DARMINO_SDK_PATH=${armino_path}
    cd ../$BUILD_DIR
    ninja -- install
else
    cmake -S . -B ../$BUILD_DIR -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Release -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER -DARMINO_SDK_PATH=${armino_path}
    cmake --build ../$BUILD_DIR -- install -j8
fi

