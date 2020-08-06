cmake_minimum_required(VERSION 3.16)

include(version.cmake)

# Frank Video: use computer video (camera) to detect shapes
project(${APP_LOW_NAME} VERSION ${APP_VERSION})

# C++ standard required = C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(videoConfig.h.in videoConfig.h)

include(sources.cmake)
include(test/all_tests.cmake)
include(third_party/dependencies.cmake)

target_link_libraries(${APP_LOW_NAME} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
if (WIN32)
  target_link_libraries(${APP_NAME_WINMAIN} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
endif()
target_link_libraries(${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)

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
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Werror -Wall -Wextra -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:/WX /Wall /w45038>)

include(install/all_installers.cmake)
