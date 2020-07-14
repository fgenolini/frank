cmake_minimum_required(VERSION 3.16)

# Use computer video (camera) to detect shapes
project(video VERSION 1.0)

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_library(video_cv opencv_ui.cpp list_devices.cpp win32_list_devices.cpp
            linux_list_devices.cpp macos_list_devices.cpp input_device.cpp
            opencv_window.cpp)

# Command line executable with a simple OpenCV GUI
add_executable(video video_main.cpp run_application.cpp)

# Command line executable with a simple OpenCV GUI
add_executable(video_test video_test_main.cpp
               test/test_list_devices.cpp
               )

# Open Computer Vision (github.com/opencv/opencv)
find_package(OpenCV REQUIRED)

# C++ Guidelines Support Library (github.com/microsoft/gsl)
# Include only, no binary, no library
find_package(Microsoft.GSL REQUIRED)

# OpenCV User Interface as a header-only library (github.com/Dovyski/cvui)
file(DOWNLOAD https://raw.githubusercontent.com/Dovyski/cvui/master/cvui.h ${CMAKE_BINARY_DIR}/cvui.h)
file(DOWNLOAD https://raw.githubusercontent.com/Dovyski/cvui/master/EnhancedWindow.h ${CMAKE_BINARY_DIR}/EnhancedWindow.h)

target_link_libraries(video video_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)
target_link_libraries(video_test video_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)
target_link_libraries(video_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)

configure_file(videoConfig.h.in videoConfig.h)
target_include_directories(video PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}"
                           )
target_include_directories(video_cv PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}"
                           )

find_file(HAVE_CATCH_HPP catch.hpp PATH_SUFFIXES catch2 catch)

if(HAVE_CATCH_HPP)
  get_filename_component(CATCH_PATH ${HAVE_CATCH_HPP} DIRECTORY)
  target_include_directories(video_test INTERFACE ${CATCH_PATH}
                             "${PROJECT_BINARY_DIR}"
                             "${CMAKE_BINARY_DIR}"
                            )
else()
  include(ExternalProject)
  find_package(Git)
  ExternalProject_Add(
    git_catch
    PREFIX ${CMAKE_BINARY_DIR}/catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
  )

  ExternalProject_Get_Property(git_catch source_dir)
  add_dependencies(video_test git_catch)
  target_include_directories(video_test PUBLIC
                             ${source_dir}/single_include
                             ${source_dir}/single_include/catch2
                             "${PROJECT_BINARY_DIR}"
                             "${CMAKE_BINARY_DIR}"
                            )
endif()

add_test(video_list_devices video_test "Scenario: frank video list devices")

install(TARGETS video RUNTIME DESTINATION bin)

include(CPack)
