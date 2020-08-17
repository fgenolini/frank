cmake_minimum_required(VERSION 3.16)

include(version.cmake)

# Frank Video: use computer video (camera) to detect shapes
project(${APP_LOW_NAME} VERSION ${APP_VERSION})

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(THIRD_PARTY_SHARED_LIBS
  "Link third party code as shared libraries (DLL or SO) instead of static libraries (LIB or A)" ON)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(videoConfig.h.in videoConfig.h)

include(sources.cmake)
if (WIN32)
else()
  include(check/tidy.cmake)
  include(check/sanitiser.cmake)
  include(test/code-coverage.cmake)
endif()
include(test/all_tests.cmake)
include(third_party/dependencies.cmake)

if (CODE_COVERAGE)
  add_code_coverage()
endif()

target_link_libraries(${APP_LOW_NAME} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
if (WIN32)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
  target_link_libraries(${APP_NAME_WINMAIN} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
endif()
target_link_libraries(${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)

target_include_directories(${APP_LOW_NAME} PUBLIC
  "${PROJECT_BINARY_DIR}" "${CMAKE_BINARY_DIR}")
if (WIN32)
  target_include_directories(${APP_NAME_WINMAIN} PUBLIC
    "${PROJECT_BINARY_DIR}" "${CMAKE_BINARY_DIR}")
endif()
target_include_directories(${LIB_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}" "${CMAKE_BINARY_DIR}")
target_compile_options(${LIB_NAME} PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>
  $<$<CXX_COMPILER_ID:MSVC>:/Wall /w45038 /wd4365 /wd4820>)

if (CODE_COVERAGE)
  add_code_coverage_all_targets(EXCLUDE
    build/cvui.h
    build/EnhancedWindow.h
    build/nlohmann/json.hpp
    build/portable-file-dialogs.h
    build/video/videoConfig.h
    ui/cvui_init.cpp
    video_winmain.cpp
    ${NO_COVERAGE_SOURCES})
  target_code_coverage(${LIB_NAME} AUTO ALL)
  target_code_coverage(${APP_LOW_NAME} AUTO ${APP_LOW_NAME})
  foreach(Test_Target ${TESTS})
    target_code_coverage(${Test_Target} AUTO ALL EXTERNAL)
  endforeach()
endif()

include(install/all_installers.cmake)
