export PATH=/opt/gcc-arm-none-eabi-9-2020-q2-update/bin:$PATH

#rm -rf cmake_build
cd tfm
current_dir=$(pwd)
armino_path=${current_dir}/../../../
#cmake -S . -B ../cmake_build -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER
cmake -S . -B ../cmake_build -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER -DARMINO_SDK_PATH=${armino_path}
cmake --build ../cmake_build -- install
