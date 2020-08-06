# Configure the CTest unit test runner.
# Unit tests for frank video

add_executable(${TEST_APP_NAME} test/${TEST_APP_NAME}_main.cpp
               test/test_list_devices.cpp)
add_executable(${TEST_APP_NAME}_main_2 test/${TEST_APP_NAME}_main.cpp
               test/test_main_2.cpp video_main.cpp)
add_executable(${TEST_APP_NAME}_run_application_2 test/${TEST_APP_NAME}_main.cpp
               test/test_run_application_2.cpp run_application.cpp)
add_executable(${TEST_APP_NAME}_opencv_ui_1
               test/${TEST_APP_NAME}_main.cpp test/test_opencv_ui_1.cpp
               test/testable_exit.cpp
               ui/opencv_ui.cpp
               device/input_device.cpp)
add_executable(${TEST_APP_NAME}_video_gui_1
               test/${TEST_APP_NAME}_main.cpp test/test_video_gui_1.cpp
               ui/video_gui.cpp
               device/input_device.cpp)
if (WIN32)
  add_executable(${TEST_APP_NAME}_winmain_4 test/${TEST_APP_NAME}_main.cpp
                 test/test_winmain_4.cpp video_winmain.cpp)
endif()

target_compile_definitions(${TEST_APP_NAME}_main_2
                           PUBLIC _TEST_MAIN_2_ _DO_NOTHING_EXIT_)
target_compile_definitions(${TEST_APP_NAME}_run_application_2
                           PUBLIC _TEST_RUN_APPLICATION_2_ _DO_NOTHING_EXIT_)
target_compile_definitions(${TEST_APP_NAME}_opencv_ui_1
                           PUBLIC _TEST_OPENCV_UI_1_ _DO_NOTHING_EXIT_)
target_compile_definitions(${TEST_APP_NAME}_video_gui_1
                           PUBLIC _TEST_VIDEO_GUI_1_ _DO_NOTHING_EXIT_)
if (WIN32)
  target_compile_definitions(${TEST_APP_NAME}_winmain_4
                             PUBLIC _TEST_WINMAIN_4_ _DO_NOTHING_EXIT_)
endif()

set(TESTS ${TEST_APP_NAME} ${TEST_APP_NAME}_main_2
    ${TEST_APP_NAME}_run_application_2 ${TEST_APP_NAME}_opencv_ui_1
    ${TEST_APP_NAME}_video_gui_1)
if (WIN32)
  list(APPEND TESTS ${TEST_APP_NAME}_winmain_4)
endif()

foreach(Test_Target ${TESTS})
  add_test(${Test_Target} ${Test_Target})
endforeach()

set(TESTS_WITH_OPENCV ${TEST_APP_NAME} ${TEST_APP_NAME}_video_gui_1)
foreach(Link_With_OpenCV ${TESTS_WITH_OPENCV})
  target_link_libraries(${Link_With_OpenCV} ${LIB_NAME} ${OpenCV_LIBS} Microsoft.GSL::GSL)
endforeach()
