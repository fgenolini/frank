# github.com/StableCoder/cmake-scripts/blob/master/tools.cmake

option(CLANG_TIDY "Turns on clang-tidy processing if it is found." OFF)
option(CPPCHECK "Turns on cppcheck processing if it is found." OFF)

macro(clang_tidy)
  if(CLANG_TIDY AND CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} ${ARGN})
  endif()
endmacro()

macro(cppcheck)
  if(CPPCHECK AND CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXE} ${ARGN})
  endif()
endmacro()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
mark_as_advanced(FORCE CLANG_TIDY_EXE)
if(CLANG_TIDY_EXE)
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  if(NOT CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
  endif()
elseif(CLANG_TIDY)
  message(SEND_ERROR "Cannot enable clang-tidy, as executable not found!")
  set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
else()
  message(STATUS "clang-tidy not found!")
  set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
endif()

find_program(CPPCHECK_EXE NAMES "cppcheck")
mark_as_advanced(FORCE CPPCHECK_EXE)
if(CPPCHECK_EXE)
  message(STATUS "cppcheck found: ${CPPCHECK_EXE}")
  if(CPPECHECK)
    set(CMAKE_CXX_CPPCHECK
        "${CPPCHECK_EXE};--enable=warning,performance,portability,missingInclude;--template=\"[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)\";--suppress=missingIncludeSystem;--quiet;--verbose;--force")
  endif()
  if(NOT CPPCHECK)
    set(CMAKE_CXX_CPPCHECK "" CACHE STRING "" FORCE)
  endif()
elseif(CPPCHECK)
  message(SEND_ERROR "Cannot enable cppcheck, as executable not found!")
  set(CMAKE_CXX_CPPCHECK "" CACHE STRING "" FORCE)
else()
  message(STATUS "cppcheck not found!")
  set(CMAKE_CXX_CPPCHECK "" CACHE STRING "" FORCE)
endif()
