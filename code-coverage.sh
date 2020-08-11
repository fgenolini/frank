#!/bin/bash
# Pre-requirements: configure project as Debug with code coverage enabled
export LIBRARY_PATH=/opt/local/lib
mkdir -p build
cd build
cmake --build . --target ccov-all
