cmake_minimum_required(VERSION 3.16)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "1")
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
    json/persisted_settings.cpp
    opencv/add_overlay.cpp opencv/exit_requested.cpp opencv/opencv_window.cpp
    opencv/paint_picture.cpp opencv/take_picture.cpp
    ui/main_settings_window.cpp ui/main_window.cpp ui/opencv_ui.cpp
    ui/other_window.cpp ui/video_gui.cpp)

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

# Command line executable with a simple OpenCV GUI
add_executable(${APP_LOW_NAME}_test test/video_test_main.cpp
               test/test_list_devices.cpp)

# ------ Start dependencies ------               
# OpenCV, Open Computer Vision (github.com/opencv/opencv)
find_package(OpenCV REQUIRED)

# OpenCV User Interface (github.com/Dovyski/cvui)
# Headers only, no binary, no library
file(DOWNLOAD
     https://raw.githubusercontent.com/Dovyski/cvui/master/cvui.h
     ${CMAKE_BINARY_DIR}/cvui.h)
file(DOWNLOAD
     https://raw.githubusercontent.com/Dovyski/cvui/master/EnhancedWindow.h
     ${CMAKE_BINARY_DIR}/EnhancedWindow.h)

# Microsoft GSL, C++ Guidelines Support Library (github.com/microsoft/gsl)
# Header only, no binary, no library
find_package(Microsoft.GSL REQUIRED)

# Trompeloeil C++ mocking
# Header only, no binary, no libray
file(DOWNLOAD
     https://raw.githubusercontent.com/rollbear/trompeloeil/master/include/trompeloeil.hpp
     ${CMAKE_BINARY_DIR}/trompeloeil.hpp)
file(DOWNLOAD
     https://raw.githubusercontent.com/rollbear/trompeloeil/master/include/catch2/trompeloeil.hpp
     ${CMAKE_BINARY_DIR}/catch2/trompeloeil.hpp)

# Portable file dialogs (github.com/samhocevar/portable-file-dialogs)
# Header only, no binary, no libray
file(DOWNLOAD
     https://raw.githubusercontent.com/samhocevar/portable-file-dialogs/master/portable-file-dialogs.h
     ${CMAKE_BINARY_DIR}/portable-file-dialogs.h)

# JSON (github.com/nlohmann/json)
# Header only, no binary, no libray
file(DOWNLOAD
     https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
     ${CMAKE_BINARY_DIR}/nlohmann/json.hpp)

# Catch 2, unit testing for C++
# Headers only, no binary, no libray
find_file(HAVE_CATCH_HPP catch.hpp PATH_SUFFIXES catch2 catch)
if(HAVE_CATCH_HPP)
  get_filename_component(CATCH_PATH ${HAVE_CATCH_HPP} DIRECTORY)
  target_include_directories(${TEST_APP_NAME} INTERFACE ${CATCH_PATH}
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
  add_dependencies(${TEST_APP_NAME} git_catch)
  target_include_directories(${TEST_APP_NAME} PUBLIC
                             ${source_dir}/single_include
                             ${source_dir}/single_include/catch2
                             "${PROJECT_BINARY_DIR}"
                             "${CMAKE_BINARY_DIR}")
endif()
# ------ End dependencies ------               

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
                           "${PROJECT_BINARY_DIR}"
                           "${CMAKE_BINARY_DIR}")
target_compile_options(${LIB_NAME} PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Werror -Wall -Wextra -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /WX /W4 /w45038>)

add_test(video_list_devices video_test "Scenario: ${ROOT_PROJECT_LOW_NAME} ${APP_LOW_NAME} list devices")

# CPack installer
include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)
include(CPackComponent)
if (WIN32)
  install(TARGETS ${APP_NAME_WINMAIN} RUNTIME DESTINATION bin COMPONENT Applications)
endif()
install(TARGETS ${APP_LOW_NAME} RUNTIME DESTINATION bin COMPONENT Applications)
install(TARGETS ${TEST_APP_NAME} RUNTIME DESTINATION bin COMPONENT Tests)

set(CPACK_COMPONENTS_ALL Applications Tests Dependencies)

if (WIN32)
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  set(CPACK_GENERATOR WIX)
  set(CPACK_WIX_UPGRADE_GUID "475EB28F-7E72-497E-8F9D-DD607B91003E")
  if(DEFINED _OpenCV_LIB_PATH)
    file(TO_NATIVE_PATH "${_OpenCV_LIB_PATH}" Installed_OpenCV_PATH)
  else()
    get_filename_component(OpenCV_BIN_PATH "${OpenCV_INSTALL_PATH}/../bin" ABSOLUTE)
    if(NOT EXISTS "${OpenCV_BIN_PATH}")
      get_filename_component(OpenCV_BIN_PATH "${OpenCV_INSTALL_PATH}/bin" ABSOLUTE)
    endif()
    file(TO_NATIVE_PATH "${OpenCV_BIN_PATH}" Installed_OpenCV_PATH)
  endif()
  set(OpenCV_VER "${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
  set(ALL_RUNTIMES "")
  foreach(OpenCV_DLL_File
          opencv_core opencv_highgui opencv_features2d opencv_imgcodecs
          opencv_imgproc opencv_objdetect opencv_photo opencv_stitching
          opencv_video opencv_videoio opencv_ml opencv_gapi opencv_flann
          opencv_dnn opencv_calib3d opencv_world)
    set(OPENCV_DLL_FILE "${Installed_OpenCV_PATH}/${OpenCV_DLL_File}${OpenCV_VER}.dll")
    if(EXISTS "${OPENCV_DLL_FILE}")
      string(REPLACE "\\" "/" OPENCV_DLL_FILE_UNIX "${OPENCV_DLL_FILE}")
      list(APPEND ALL_RUNTIMES "${OPENCV_DLL_FILE_UNIX}")
    endif()
  endforeach()

  foreach(Extra_OpenCV_DLL_File
          opencv_videoio_ffmpeg opencv_videoio_msmf)
    set(EXTRA_OPENCV_DLL_FILE "${Installed_OpenCV_PATH}/${Extra_OpenCV_DLL_File}${OpenCV_VER}_64.dll")
    if(EXISTS "${EXTRA_OPENCV_DLL_FILE}")
      string(REPLACE "\\" "/" EXTRA_OPENCV_DLL_FILE_UNIX "${EXTRA_OPENCV_DLL_FILE}")
      list(APPEND ALL_RUNTIMES "${EXTRA_OPENCV_DLL_FILE_UNIX}")
    endif()
  endforeach()

  foreach(OpenCV_DLL_Support
          tbb tbbbind tbbmalloc ngraph clDNNPlugin HeteroPlugin
          inference_engine inference_engine_ir_reader inference_engine_legacy
          inference_engine_lp_transformations inference_engine_transformations
          libhwloc-5 MKLDNNPlugin myriadPlugin)
    set(OPENCV_SUPPORT_FILE "${Installed_OpenCV_PATH}/${OpenCV_DLL_Support}.dll")
    if(EXISTS "${OPENCV_SUPPORT_FILE}")
      string(REPLACE "\\" "/" OPENCV_SUPPORT_FILE_UNIX "${OPENCV_SUPPORT_FILE}")
      list(APPEND ALL_RUNTIMES "${OPENCV_SUPPORT_FILE_UNIX}")
    endif()
  endforeach()
  install(FILES ${ALL_RUNTIMES} DESTINATION bin COMPONENT Dependencies)
  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
          DESTINATION bin COMPONENT Dependencies)

  set(CPACK_ALL_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_APPLICATIONS_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_DEPENDENCIES_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_TESTS_INSTALL_TYPES Full)
  set(CPACK_WIX_PROGRAM_MENU_FOLDER "${ROOT_PROJECT_NAME} ${APP_NAME}")
  set(CPACK_PACKAGE_EXECUTABLES ${APP_NAME_WINMAIN} "${ROOT_PROJECT_NAME} ${APP_NAME}")
  set_property(INSTALL "bin/${APP_NAME_WINMAIN}.exe"
               PROPERTY CPACK_START_MENU_SHORTCUTS "${ROOT_PROJECT_NAME} ${APP_NAME}")
endif()

set(CPACK_PACKAGE_VENDOR "fgenolini")
set(CPACK_PACKAGE_NAME "${ROOT_PROJECT_LOW_NAME}-${APP_LOW_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "${ROOT_PROJECT_NAME} ${APP_NAME} - viewer for 4 webcams with edge detection")
set(CPACK_PACKAGE_VERSION ${APP_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${ROOT_PROJECT_LOW_NAME}_${APP_LOW_NAME}")
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "GUI applications")
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
    "OpenCV GUI viewer for up to 4 video input devices (webcams), with optional edge detection")
set(CPACK_COMPONENT_DEPENDENCIES_DISPLAY_NAME "System dependencies")
set(CPACK_COMPONENT_DEPENDENCIES_DESCRIPTION
    "DLLs for OpenCV and Microsoft C++")
set(CPACK_COMPONENT_TESTS_DISPLAY_NAME "Unit tests")
set(CPACK_COMPONENT_TESTS_DESCRIPTION
    "Tests and checks for all the ${ROOT_PROJECT_NAME} ${APP_NAME} applications, for software quality assurance")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

include(CPack)
