# C++ source files

set(CommonSourcesList
  device/input_device.cpp device/list_devices.cpp
  file/file_dialogs.cpp
  json/persisted_settings.cpp
  opencv/add_overlay.cpp opencv/exit_requested.cpp opencv/opencv_window.cpp
  opencv/paint_histogram.cpp opencv/paint_picture.cpp opencv/take_picture.cpp
  test/testable_exit.cpp
  ui/main_settings_window.cpp ui/main_window.cpp
  ui/other_window.cpp ui/run_ui.cpp ui/statistics_window.cpp ui/video_gui.cpp)

set(LIBRARY_TYPE STATIC)

if (WIN32)
  add_library(${LIB_NAME} ${LIBRARY_TYPE} device/win32_list_devices.cpp
    ${CommonSourcesList})
  if(NOT ${THIRD_PARTY_SHARED_LIBS})
    set_property(TARGET ${LIB_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
elseif(APPLE AND UNIX)
  add_library(${LIB_NAME} ${LIBRARY_TYPE} device/macos_list_devices.cpp
    ${CommonSourcesList})
elseif(UNIX AND (NOT APPLE) AND (NOT MINGW) AND (NOT MSYS) AND (NOT CYGWIN))
  add_library(${LIB_NAME} ${LIBRARY_TYPE} device/linux_list_devices.cpp
    ${CommonSourcesList})
else()
  add_library(${LIB_NAME} ${LIBRARY_TYPE} ${CommonSourcesList})
endif()

# Command line executable with a simple OpenCV GUI
add_executable(${APP_LOW_NAME} video_main.cpp run_application.cpp)
if (WIN32)
  if(NOT ${THIRD_PARTY_SHARED_LIBS})
    set_property(TARGET ${APP_LOW_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()

  # WinMain instead of main, so that no DOS window is shown for Windows users
  add_executable(${APP_NAME_WINMAIN} WIN32 video_winmain.cpp run_application.cpp)
  if(NOT ${THIRD_PARTY_SHARED_LIBS})
    set_property(TARGET ${APP_NAME_WINMAIN} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()
