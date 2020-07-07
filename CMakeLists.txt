# frank: C++ project for cooperative programming
# Using OpenCV, Microsoft GSL
#
# TO DO:
# Manually install OpenCV and Microsoft GSL before configuring this C++ project
#
# For an example of how to build on Linux
# (Ubuntu 20.04, with OpenCV 4.2.0, and the latest Microsoft GSL from github master),
# see the github CI action on this repository

cmake_minimum_required(VERSION 3.17)

project(frank VERSION 1.0.0)

include(CTest)
enable_testing()

# Computer vision using OpenCV
add_subdirectory(video)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
