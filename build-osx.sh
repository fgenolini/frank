#!/bin/bash
export LIBRARY_PATH=/opt/local/lib
mkdir -p build
cd build
cmake  -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
ctest
