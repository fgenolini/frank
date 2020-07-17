cmake_minimum_required(VERSION 3.16)

# Use computer video (camera) to detect shapes
project(video VERSION 1.0.0)

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

if (WIN32)
  message("Building for Microsoft Windows")
  add_library(video_cv opencv_ui.cpp list_devices.cpp win32_list_devices.cpp
              input_device.cpp opencv_window.cpp)
elseif(APPLE AND UNIX)
  message("Building for Mac OSX")
  add_library(video_cv opencv_ui.cpp list_devices.cpp macos_list_devices.cpp
              input_device.cpp opencv_window.cpp)
elseif(UNIX AND (NOT APPLE) AND (NOT MINGW) AND (NOT MSYS) AND (NOT CYGWIN))
  message("Building for Linux")
  add_library(video_cv opencv_ui.cpp list_devices.cpp linux_list_devices.cpp
              input_device.cpp opencv_window.cpp)
else()
  message("Building for other")
  add_library(video_cv opencv_ui.cpp list_devices.cpp
              input_device.cpp opencv_window.cpp)
endif()

# Command line executable with a simple OpenCV GUI
add_executable(video video_main.cpp run_application.cpp)

# Command line executable with a simple OpenCV GUI
add_executable(video_test video_test_main.cpp
               test/test_list_devices.cpp)

# ------ Start dependencies ------               
# OpenCV, Open Computer Vision (github.com/opencv/opencv)
find_package(OpenCV REQUIRED)

# OpenCV User Interface, as a header-only library (github.com/Dovyski/cvui)
file(DOWNLOAD
     https://raw.githubusercontent.com/Dovyski/cvui/master/cvui.h
     ${CMAKE_BINARY_DIR}/cvui.h)
file(DOWNLOAD
     https://raw.githubusercontent.com/Dovyski/cvui/master/EnhancedWindow.h
     ${CMAKE_BINARY_DIR}/EnhancedWindow.h)

# Microsoft GSL, C++ Guidelines Support Library (github.com/microsoft/gsl)
# Include only, no binary, no library
find_package(Microsoft.GSL REQUIRED)

# Trompeloeil C++ mocking, as a header-only library
file(DOWNLOAD
     https://raw.githubusercontent.com/rollbear/trompeloeil/master/include/trompeloeil.hpp
     ${CMAKE_BINARY_DIR}/trompeloeil.hpp)
file(DOWNLOAD
     https://raw.githubusercontent.com/rollbear/trompeloeil/master/include/catch2/trompeloeil.hpp
     ${CMAKE_BINARY_DIR}/catch2/trompeloeil.hpp)

# Catch 2, unit testing for C++
find_file(HAVE_CATCH_HPP catch.hpp PATH_SUFFIXES catch2 catch)
if(HAVE_CATCH_HPP)
  get_filename_component(CATCH_PATH ${HAVE_CATCH_HPP} DIRECTORY)
  target_include_directories(video_test INTERFACE ${CATCH_PATH}
                             "${PROJECT_BINARY_DIR}"
                             "${CMAKE_BINARY_DIR}")
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
    LOG_DOWNLOAD ON)

  ExternalProject_Get_Property(git_catch source_dir)
  add_dependencies(video_test git_catch)
  target_include_directories(video_test PUBLIC
                             ${source_dir}/single_include
                             ${source_dir}/single_include/catch2
                             "${PROJECT_BINARY_DIR}"
                             "${CMAKE_BINARY_DIR}")
endif()
# ------ End dependencies ------               

target_link_libraries(video video_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)
target_link_libraries(video_test video_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)
target_link_libraries(video_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)

configure_file(videoConfig.h.in videoConfig.h)
target_include_directories(video PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}")
target_include_directories(video_cv PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}")

add_test(video_list_devices video_test "Scenario: frank video list devices")

# CPack installer
install(TARGETS video RUNTIME DESTINATION bin COMPONENT Applications)
install(TARGETS video_test RUNTIME DESTINATION bin COMPONENT Tests)

set(CPACK_COMPONENTS_ALL Applications Tests)

if (WIN32)
  message("Configuring NSIS installer for Microsoft Windows")
  message("Using Visual Studio ${MSVC_VERSION}, toolset ${MSVC_TOOLSET_VERSION}")

  set(CPACK_GENERATOR NSIS)
  set(CPACK_ALL_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_APPLICATIONS_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_TESTS_INSTALL_TYPES Full)
  include(InstallRequiredSystemLibraries)
endif()

set(CPACK_PACKAGE_NAME "FrankVideo")
set(CPACK_PACKAGE_VENDOR "github fgenolini frank")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "frank video - viewer for 4 webcams with edge detection")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "frank video")
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "frank applications")
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
    "OpenCV GUI viewer for up to 4 video input devices (webcams), with optional edge detection")
set(CPACK_COMPONENT_TESTS_DESCRIPTION
    "Tests and checks for all the frank applications, for software quality assurance")

include(CPack)
