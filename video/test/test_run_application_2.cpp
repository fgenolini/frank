#include "config.h"

#ifndef _TEST_RUN_APPLICATION_2_
#error _TEST_RUN_APPLICATION_2_ not defined
#endif

WARNINGS_OFF
#include <iostream>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/list_devices.h"
#include "run_application.h"
#include "ui/opencv_ui.h"

namespace test::frank {

class run_application_mock {
public:
  bool list_input_devices_called = false;
  bool opencv_ui_called = false;
};

std::vector<::frank::video::input_device>
mocked_list_input_devices(::frank::video::device_register const *,
                          run_application_mock *mock) {
  if (!mock) {
    std::cerr << "mocked_list_input_devices: no mock\n";
    return std::vector<::frank::video::input_device>();
  }

  mock->list_input_devices_called = true;
  return std::vector<::frank::video::input_device>();
}

void mocked_opencv_ui(std::vector<::frank::video::input_device> &,
                      run_application_mock *mock) {
  if (!mock) {
    std::cerr << "mocked_opencv_ui: no mock\n";
    return;
  }

  mock->opencv_ui_called = true;
}

} // namespace test::frank

namespace frank::video {

std::vector<input_device>
list_input_devices(device_register const *name_devices, void *mock_data) {
  if (!mock_data) {
    std::cerr << "list_input_devices: no mock\n";
    return test::frank::mocked_list_input_devices(name_devices, nullptr);
  }

  auto mock = static_cast<::test::frank::run_application_mock *>(mock_data);
  return test::frank::mocked_list_input_devices(name_devices, mock);
}

void opencv_ui(std::vector<input_device> &connected_webcams,
               user_interface_factory, void *mock_data) {
  if (!mock_data) {
    std::cerr << "opencv_ui: no mock\n";
    test::frank::mocked_opencv_ui(connected_webcams, nullptr);
    return;
  }

  auto mock = static_cast<::test::frank::run_application_mock *>(mock_data);
  test::frank::mocked_opencv_ui(connected_webcams, mock);
}

} // namespace frank::video

SCENARIO("frank video run application 2", "[run_application_2]") {
  GIVEN("run the frank video application") {
    WHEN("no argument") {
      test::frank::run_application_mock mock{};

      frank::video::run_application(0, nullptr, &mock);

      THEN("list_input_devices is called") {
        REQUIRE(mock.list_input_devices_called == true);
      }

      AND_THEN("opencv_ui is called") {
        REQUIRE(mock.opencv_ui_called == true);
      }
    }
  }
}
