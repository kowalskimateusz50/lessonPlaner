#!/usr/bin/env bash
rm -dr build &&
mkdir build &&
cd build &&
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake &&
cmake --build . &&
cd ..
