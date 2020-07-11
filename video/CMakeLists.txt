cmake_minimum_required(VERSION 3.16)

# Use computer video (camera) to detect shapes
project(video VERSION 1.0)

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Command line executable with a simple OpenCV GUI
add_executable(video video_main.cpp run_tests.cpp
               list_devices.cpp opencv_ui.cpp
               input_device.cpp opencv_window.cpp)

# Open Computer Vision (github.com/opencv/opencv)
find_package(OpenCV REQUIRED)

# C++ Guidelines Support Library (github.com/microsoft/gsl)
# Include only, no binary, no library
find_package(Microsoft.GSL REQUIRED)

# OpenCV User Interface as a header-only library (github.com/Dovyski/cvui)
file(DOWNLOAD https://raw.githubusercontent.com/Dovyski/cvui/master/cvui.h ${CMAKE_BINARY_DIR}/cvui.h)
file(DOWNLOAD https://raw.githubusercontent.com/Dovyski/cvui/master/EnhancedWindow.h ${CMAKE_BINARY_DIR}/EnhancedWindow.h)

target_link_libraries(video ${OpenCV_LIBS} Microsoft.GSL::GSL)

configure_file(videoConfig.h.in videoConfig.h)
target_include_directories(video PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}"
                           )

add_test(null_test video test null)
