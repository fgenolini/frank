#!/bin/bash
sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu focal-security main"
sudo apt-get update
sudo apt-get remove x264 libx264-dev -y
# In the .travis.yml file:
# sudo apt-get install -y build-essential checkinstall cmake pkg-config git libopencv-dev python3-opencv

