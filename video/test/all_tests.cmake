# Configure the CTest unit test runner.
# Unit tests for frank video

# Catch2 test runner application
set(TEST_APP_NAME "${APP_LOW_NAME}_test")

add_executable(${TEST_APP_NAME}_exceptions_handler
  test/catch_implementation.cpp
  test/test_exceptions_handler.cpp exception/exceptions.cpp)
add_executable(${TEST_APP_NAME}_list_devices
  test/catch_implementation.cpp
  test/test_list_devices.cpp test/testable_cstdio.cpp
  device/input_device.cpp device/linux_list_devices.cpp device/video_devices.cpp
  device/macos_list_devices.cpp device/win32_list_devices.cpp)
add_executable(${TEST_APP_NAME}_main_2
  test/catch_implementation.cpp
  test/test_main_2.cpp video_main.cpp)
add_executable(${TEST_APP_NAME}_main_settings_window_draw_4
  test/catch_implementation.cpp
  test/test_main_settings_window_draw_4.cpp)
add_executable(${TEST_APP_NAME}_open_file_3
  test/catch_implementation.cpp
  test/test_open_file_3.cpp file/file_dialogs.cpp)
add_executable(${TEST_APP_NAME}_run_application_2
  test/catch_implementation.cpp
  test/test_run_application_2.cpp application.cpp)
add_executable(${TEST_APP_NAME}_run_ui_3
  test/catch_implementation.cpp
  test/test_run_ui_3.cpp
  ui/ui.cpp
  device/input_device.cpp)
add_executable(${TEST_APP_NAME}_video_gui_1
  test/catch_implementation.cpp
  test/test_video_gui_1.cpp)
if (WIN32)
  add_executable(${TEST_APP_NAME}_winmain_4
    test/catch_implementation.cpp
    test/test_winmain_4.cpp video_winmain.cpp)
endif()

target_compile_definitions(${TEST_APP_NAME}_exceptions_handler PUBLIC
  _TEST_EXCEPTIONS_HANDLER_ _DO_NOTHING_EXIT_)
target_compile_definitions(${TEST_APP_NAME}_list_devices PUBLIC
  _TEST_LIST_DEVICES_ _DO_NOTHING_STDIO_)
target_compile_definitions(${TEST_APP_NAME}_main_2 PUBLIC
  _TEST_MAIN_2_ _DO_NOTHING_EXIT_)
target_compile_definitions(${TEST_APP_NAME}_main_settings_window_draw_4 PUBLIC
  _TEST_MAIN_SETTINGS_WINDOW_DRAW_4_ _DO_NOTHING_EXIT_ _FAKE_FILE_DIALOGS_)
target_compile_definitions(${TEST_APP_NAME}_open_file_3 PUBLIC
  _TEST_OPEN_FILE_3_ _DO_NOTHING_EXIT_ _FAKE_FILE_DIALOGS_)
target_compile_definitions(${TEST_APP_NAME}_run_application_2 PUBLIC
  _TEST_RUN_APPLICATION_2_ _DO_NOTHING_EXIT_)
target_compile_definitions(${TEST_APP_NAME}_run_ui_3 PUBLIC
  _TEST_RUN_UI_3_ _DO_NOTHING_EXIT_)
target_compile_definitions(${TEST_APP_NAME}_video_gui_1 PUBLIC
  _TEST_VIDEO_GUI_1_ _DO_NOTHING_EXIT_)
if (WIN32)
  target_compile_definitions(${TEST_APP_NAME}_winmain_4 PUBLIC
    _TEST_WINMAIN_4_ _DO_NOTHING_EXIT_)
endif()

set(TESTS
  ${TEST_APP_NAME}_exceptions_handler
  ${TEST_APP_NAME}_list_devices
  ${TEST_APP_NAME}_main_2
  ${TEST_APP_NAME}_main_settings_window_draw_4
  ${TEST_APP_NAME}_open_file_3
  ${TEST_APP_NAME}_run_application_2
  ${TEST_APP_NAME}_run_ui_3
  ${TEST_APP_NAME}_video_gui_1)
if (WIN32)
  list(APPEND TESTS ${TEST_APP_NAME}_winmain_4)
endif()

set(NO_COVERAGE_SOURCES
  build/catch2/trompeloeil.hpp
  build/trompeloeil.hpp
  test/test_exceptions_handler.cpp
  test/test_list_devices.cpp
  test/test_main_2.cpp
  test/test_main_settings_window_draw_4.cpp
  test/test_open_file_3.cpp
  test/test_run_application_2.cpp
  test/test_run_ui_3.cpp
  test/test_video_gui_1.cpp
  test/testable_exit.cpp
  )

foreach(Test_Target ${TESTS})
  if (WIN32)
    if(NOT ${THIRD_PARTY_SHARED_LIBS})
      set_property(TARGET ${Test_Target} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
  endif()
  
  target_compile_options(${Test_Target} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/Wall /wd4365 /wd4868 /wd5038 /wd4820>)
  add_test(${Test_Target} ${Test_Target})
endforeach()

set(TESTS_WITH_GSL ${TEST_APP_NAME}_list_devices)
foreach(Link_With_Gsl ${TESTS_WITH_GSL})
  target_link_libraries(${Link_With_Gsl} Microsoft.GSL::GSL)
endforeach()

set(TESTS_WITH_OPENCV_AND_LIB
  ${TEST_APP_NAME}_main_settings_window_draw_4 ${TEST_APP_NAME}_video_gui_1)
foreach(Link_With_OpenCV_And_Lib ${TESTS_WITH_OPENCV_AND_LIB})
  target_link_libraries(${Link_With_OpenCV_And_Lib} ${LIB_NAME} ${OpenCV_LIBS}
    Microsoft.GSL::GSL)
endforeach()
