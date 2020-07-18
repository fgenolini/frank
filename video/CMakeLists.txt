cmake_minimum_required(VERSION 3.16)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(APP_VERSION
    "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(APP_NAME "video")
set(APP_LOW_NAME "video")

# Use computer video (camera) to detect shapes
project(${APP_NAME} VERSION ${APP_VERSION})

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

if (WIN32)
  add_library(${APP_NAME}_cv opencv_ui.cpp list_devices.cpp win32_list_devices.cpp
              input_device.cpp opencv_window.cpp)
  set(EXE_NAME ${APP_NAME}.exe)
elseif(APPLE AND UNIX)
  add_library(${APP_NAME}_cv opencv_ui.cpp list_devices.cpp macos_list_devices.cpp
              input_device.cpp opencv_window.cpp)
elseif(UNIX AND (NOT APPLE) AND (NOT MINGW) AND (NOT MSYS) AND (NOT CYGWIN))
  add_library(${APP_NAME}_cv opencv_ui.cpp list_devices.cpp linux_list_devices.cpp
              input_device.cpp opencv_window.cpp)
else()
  add_library(${APP_NAME}_cv opencv_ui.cpp list_devices.cpp
              input_device.cpp opencv_window.cpp)
endif()

# Command line executable with a simple OpenCV GUI
add_executable(${APP_NAME} video_main.cpp run_application.cpp)

# Command line executable with a simple OpenCV GUI
add_executable(${APP_NAME}_test video_test_main.cpp
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
  target_include_directories(${APP_NAME}_test INTERFACE ${CATCH_PATH}
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
  add_dependencies(${APP_NAME}_test git_catch)
  target_include_directories(${APP_NAME}_test PUBLIC
                             ${source_dir}/single_include
                             ${source_dir}/single_include/catch2
                             "${PROJECT_BINARY_DIR}"
                             "${CMAKE_BINARY_DIR}")
endif()
# ------ End dependencies ------               

target_link_libraries(${APP_NAME} ${APP_NAME}_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)
target_link_libraries(${APP_NAME}_test ${APP_NAME}_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)
target_link_libraries(${APP_NAME}_cv ${OpenCV_LIBS} Microsoft.GSL::GSL)

configure_file(videoConfig.h.in videoConfig.h)
target_include_directories(${APP_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}")
target_include_directories(${APP_NAME}_cv PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}")

add_test(video_list_devices video_test "Scenario: frank video list devices")

# CPack installer
include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)
include(CPackComponent)
install(TARGETS ${APP_NAME} RUNTIME DESTINATION bin COMPONENT Applications)
install(TARGETS ${APP_NAME}_test RUNTIME DESTINATION bin COMPONENT Tests)

set(CPACK_COMPONENTS_ALL Applications Tests Dependencies)

if (WIN32)
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  set(CPACK_GENERATOR WIX)
  set(CPACK_WIX_UPGRADE_GUID "475EB28F-7E72-497E-8F9D-DD607B91003E")
  string(REPLACE "\\" "/" Installed_OpenCV_PATH "${_OpenCV_LIB_PATH}")
  set(OpenCV_VER "${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
  set(ALL_RUNTIMES "")
  foreach(OpenCV_DLL_File in LISTS
          opencv_core opencv_highgui opencv_features2d opencv_imgcodecs
          opencv_imgproc opencv_objdetect opencv_photo opencv_stitching
          opencv_video opencv_videoio opencv_ml opencv_gapi opencv_flann
          opencv_dnn opencv_calib3d)
    if(EXISTS "${Installed_OpenCV_PATH}/${OpenCV_DLL_File}${OpenCV_VER}.dll")
      list(APPEND ALL_RUNTIMES
           "${Installed_OpenCV_PATH}/${OpenCV_DLL_File}${OpenCV_VER}.dll")
    endif()
  endforeach()
  if(EXISTS "${Installed_OpenCV_PATH}/opencv_videoio_ffmpeg${OpenCV_VER}_64.dll")
    list(APPEND ALL_RUNTIMES
         "${Installed_OpenCV_PATH}/opencv_videoio_ffmpeg${OpenCV_VER}_64.dll")
  endif()
  install(FILES ${ALL_RUNTIMES} DESTINATION bin COMPONENT Dependencies)
  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
          DESTINATION bin COMPONENT Dependencies)

  set(CPACK_ALL_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_APPLICATIONS_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_DEPENDENCIES_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_TESTS_INSTALL_TYPES Full)
  set(CPACK_WIX_PROGRAM_MENU_FOLDER "frank video")
  set(CPACK_PACKAGE_EXECUTABLES ${APP_NAME} "frank ${APP_NAME}")
  set_property(INSTALL "bin/${APP_NAME}.exe"
               PROPERTY CPACK_START_MENU_SHORTCUTS "frank video")
endif()

set(CPACK_PACKAGE_VENDOR "fgenolini")
set(CPACK_PACKAGE_NAME "frank-${APP_LOW_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "frank ${APP_NAME} - viewer for 4 webcams with edge detection")
set(CPACK_PACKAGE_VERSION ${APP_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "frank ${APP_NAME}")
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "GUI applications")
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
    "OpenCV GUI viewer for up to 4 video input devices (webcams), with optional edge detection")
set(CPACK_COMPONENT_DEPENDENCIES_DISPLAY_NAME "System dependencies")
set(CPACK_COMPONENT_DEPENDENCIES_DESCRIPTION
    "DLLs for OpenCV and Microsoft C++")
set(CPACK_COMPONENT_TESTS_DISPLAY_NAME "Unit tests")
set(CPACK_COMPONENT_TESTS_DESCRIPTION
    "Tests and checks for all the frank applications, for software quality assurance")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

include(CPack)
