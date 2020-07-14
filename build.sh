#!/bin/bash
mkdir -p build
cd build
/usr/bin/cmake ${CMAKE_OPTIONS} -DCMAKE_CXX_FLAGS=${CXX_FLAGS} -DCMAKE_BUILD_TYPE=Release ..
/usr/bin/cmake --build . --config Release
ctest
