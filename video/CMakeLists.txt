cmake_minimum_required(VERSION 3.17)

# Use computer video (camera) to detect shapes
project(video VERSION 1.0)

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Command line executable
add_executable(video video_main.cpp list_devices.cpp)

# Open Computer Vision (github.com/opencv/opencv)
find_package(OpenCV REQUIRED)

# C++ Guidelines Support Library (github.com/microsoft/gsl)
find_package(Microsoft.GSL CONFIG)

target_link_libraries(video ${OpenCV_LIBS} Microsoft.GSL::GSL)

configure_file(videoConfig.h.in videoConfig.h)
target_include_directories(video PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
