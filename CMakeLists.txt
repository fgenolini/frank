# A C++ project called frank, using OpenCV
#
# TO DO:
# Manually install OpenCV and Microsoft GSL before configuring this C++ project

cmake_minimum_required(VERSION 3.17)

project(frank VERSION 1.0.0)

include(CTest)
enable_testing()

# Computer vision using OpenCV
add_subdirectory(video)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
