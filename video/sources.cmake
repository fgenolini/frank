# C++ source files

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
  add_library(${LIB_NAME} ${CommonSourcesList})
endif()

# Command line executable with a simple OpenCV GUI
add_executable(${APP_LOW_NAME} video_main.cpp run_application.cpp)
if (WIN32)
  # WinMain instead of main, so that no DOS window is shown for Windows users
  add_executable(${APP_NAME_WINMAIN} WIN32 video_winmain.cpp run_application.cpp)
endif()
