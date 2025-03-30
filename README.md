-------------------------- Building and compiling OpenXLSX --------------------------------------
git clone https://github.com/troldal/OpenXLSX.git
cd OpenXLSX
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/absolute/path/to/install
cmake --build . --target install

/absolute/path/to/install/lib/cmake/OpenXLSX

OpenXLSXConfig.cmake
OpenXLSXConfigVersion.cmake
