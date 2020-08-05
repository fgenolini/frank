# Configure dependencies               

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
  foreach(Test_Target ${TESTS})
    target_include_directories(${Test_Target} INTERFACE ${CATCH_PATH}
                               ${OpenCV_INCLUDE_DIRS}
                               "${CMAKE_CURRENT_SOURCE_DIR}"
                               "${PROJECT_BINARY_DIR}"
                               "${CMAKE_BINARY_DIR}")
  endforeach()
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
  foreach(Test_Target ${TESTS})
    add_dependencies(${Test_Target} git_catch)
    target_include_directories(${Test_Target} PUBLIC
                               ${source_dir}/single_include
                               ${source_dir}/single_include/catch2
                               ${OpenCV_INCLUDE_DIRS}
                               "${CMAKE_CURRENT_SOURCE_DIR}"
                               "${PROJECT_BINARY_DIR}"
                               "${CMAKE_BINARY_DIR}")
  endforeach()
endif()
