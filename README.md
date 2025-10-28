-------------------------- Building and compiling project for windows --------------------------------------

rm -dr build &&
mkdir build &&
cd build &&
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake &&
cmake --build . &&
cd ..

------------------------------ Class Design -------------------------------------------------------------
