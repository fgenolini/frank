#!/bin/bash
curl -sL https://github.com/microsoft/GSL/archive/master.zip > GSL.zip
unzip -q GSL.zip
cd GSL-master
mkdir -p build
cd build
/usr/bin/cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_CXX_COMPILER=g++-9 \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      ..
make
sudo make install
cd ..
cd ..
