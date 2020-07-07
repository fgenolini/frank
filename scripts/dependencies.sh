#!/bin/bash
sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu focal-security main"
sudo apt-get update
sudo apt-get remove x264 libx264-dev -y
# In the .travis.yml file:
# sudo apt-get install -y build-essential checkinstall cmake pkg-config git libopencv-dev python3-opencv
sudo apt upgrade cmake

pwd
curl -sL https://github.com/microsoft/GSL/archive/master.zip > GSL.zip
unzip -q GSL.zip

cd GSL-master
mkdir -p build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_CXX_COMPILER=g++-9 \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      ..
make
sudo make install
cd ..
cd ..
pwd
