# C++ source files

set(CommonSourcesList
  device/input_device.cpp device/video_devices.cpp
  exception/exceptions.cpp
  file/file_dialogs.cpp
  json/persisted_settings.cpp
  opencv/add_overlay.cpp opencv/exit_requested.cpp opencv/open_webcam.cpp
  opencv/opencv_window.cpp opencv/paint_histogram.cpp opencv/paint_picture.cpp
  opencv/reduce_resolution.cpp opencv/take_picture.cpp
  test/testable_cstdio.cpp test/testable_exit.cpp
  ui/application_state.cpp ui/button_command.cpp ui/checkbox_command.cpp
  ui/cvui_init.cpp ui/layout_command.cpp ui/main_settings_window_constructor.cpp
  ui/main_settings_window_draw.cpp ui/main_settings_window_draw_canny.cpp
  ui/main_settings_window_draw_overlay.cpp
  ui/main_settings_window_draw_webcam.cpp ui/main_window.cpp ui/other_window.cpp
  ui/statistics_window.cpp ui/text_command.cpp ui/trackbar_double_command.cpp
  ui/trackbar_int_command.cpp ui/ui.cpp ui/ui_controls.cpp ui/video_gui_loop.cpp
  ui/video_gui_constructor.cpp ui/video_gui_settings.cpp
  )

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
add_executable(${APP_LOW_NAME} video_main.cpp application.cpp)
if (WIN32)
  if(NOT ${THIRD_PARTY_SHARED_LIBS})
    set_property(TARGET ${APP_LOW_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()

  # WinMain instead of main, so that no DOS window is shown for Windows users
  add_executable(${APP_NAME_WINMAIN} WIN32 video_winmain.cpp application.cpp)
  if(NOT ${THIRD_PARTY_SHARED_LIBS})
    set_property(TARGET ${APP_NAME_WINMAIN} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()
