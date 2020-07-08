#!/bin/bash
export LIBRARY_PATH=/opt/local/lib
mkdir -p build
cd build
cmake ..
cmake --build .

