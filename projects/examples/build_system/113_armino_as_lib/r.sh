rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../../../../tools/build_tools/cmake/toolchain-bk7231n.cmake -DTOOLCHAIN_DIR=/opt/gcc-arm-none-eabi-5_4-2016q3/bin -DTARGET=bk7231n -GNinja
cmake --build .

