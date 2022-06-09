export PATH=/opt/gcc-arm-none-eabi-10.3-2021.10/bin:$PATH

#rm -rf cmake_build
cd tfm
cmake -S . -B ../cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER
cmake --build ../cmake_build -- install
