cmake_minimum_required(VERSION 3.17.0)

project(frank VERSION 1.0.0)

include(CTest)
enable_testing()

add_subdirectory(video)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
