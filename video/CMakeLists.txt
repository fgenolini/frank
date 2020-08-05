cmake_minimum_required(VERSION 3.16)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(APP_VERSION
    "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(ROOT_PROJECT_NAME "Frank")
string(TOLOWER ${ROOT_PROJECT_NAME} ROOT_PROJECT_LOW_NAME)
set(APP_NAME "Video")
string(TOLOWER ${APP_NAME} APP_LOW_NAME)

# Microsoft Windows executable application that has no attached DOS command line window
set(APP_NAME_WINMAIN "${APP_LOW_NAME}w")

# Catch2 test runner application
set(TEST_APP_NAME "${APP_LOW_NAME}_test")

# Library that contains most of the source code for frank video
set(LIB_NAME "${APP_LOW_NAME}_cv")

# Use computer video (camera) to detect shapes
project(${APP_LOW_NAME} VERSION ${APP_VERSION})

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(CommonSourcesList
    device/input_device.cpp device/list_devices.cpp
    file/file_dialogs.cpp
    json/persisted_settings.cpp
    opencv/add_overlay.cpp opencv/exit_requested.cpp opencv/opencv_window.cpp
    opencv/paint_histogram.cpp opencv/paint_picture.cpp opencv/take_picture.cpp
    test/testable_exit.cpp
    ui/main_settings_window.cpp ui/main_window.cpp ui/opencv_ui.cpp
    ui/other_window.cpp ui/statistics_window.cpp ui/video_gui.cpp)

if (WIN32)
  add_library(${LIB_NAME} device/win32_list_devices.cpp
              ${CommonSourcesList})
elseif(APPLE AND UNIX)
  add_library(${LIB_NAME} device/macos_list_devices.cpp
              ${CommonSourcesList})
elseif(UNIX AND (NOT APPLE) AND (NOT MINGW) AND (NOT MSYS) AND (NOT CYGWIN))
  add_library(${LIB_NAME} device/linux_list_devices.cpp
              ${CommonSourcesList})
else()
  add_library(${LIB_NAME}
              ${CommonSourcesList})
endif()

# Command line executable with a simple OpenCV GUI
add_executable(${APP_LOW_NAME} video_main.cpp run_application.cpp)
if (WIN32)
  # WinMain instead of main, so that no DOS window is shown for Windows users
  add_executable(${APP_NAME_WINMAIN} WIN32 video_winmain.cpp run_application.cpp)
endif()

include(test/all_tests.cmake)
include(third_party/dependencies.cmake)

target_link_libraries(${APP_LOW_NAME} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
if (WIN32)
  target_link_libraries(${APP_NAME_WINMAIN} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
endif()
target_link_libraries(${TEST_APP_NAME} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
target_link_libraries(${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)

configure_file(videoConfig.h.in videoConfig.h)
target_include_directories(${APP_LOW_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}")
if (WIN32)
  target_include_directories(${APP_NAME_WINMAIN} PUBLIC
                             "${PROJECT_BINARY_DIR}"
                             "${CMAKE_BINARY_DIR}")
endif()
target_include_directories(${LIB_NAME} PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}")
target_compile_options(${LIB_NAME} PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Werror -Wall -Wextra -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /WX /Wall /w45038>)

include(install/all_installers.cmake)
