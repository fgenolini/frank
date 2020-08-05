#include "config.h"

#ifndef _TEST_RUN_APPLICATION_2_
#error _TEST_RUN_APPLICATION_2_ not defined
#endif

#include <catch2/catch.hpp>

#include "device/list_devices.h"
#include "run_application.h"
#include "ui/opencv_ui.h"

namespace test::frank {

static auto mocked_list_input_devices_called = false;
static auto mocked_opencv_ui_called = false;

std::vector<::frank::video::input_device>
mocked_list_input_devices(::frank::video::device_register const *name_devices) {
  mocked_list_input_devices_called = true;
  return std::vector<::frank::video::input_device>();
}

void mocked_opencv_ui(
    std::vector<::frank::video::input_device> &connected_webcams) {
  mocked_opencv_ui_called = true;
}

} // namespace test::frank

namespace frank::video {

std::vector<input_device>
list_input_devices(device_register const *name_devices) {
  return test::frank::mocked_list_input_devices(name_devices);
}

void opencv_ui(std::vector<input_device> &connected_webcams,
               user_interface_factory, void *) {
  test::frank::mocked_opencv_ui(connected_webcams);
}

} // namespace frank::video

SCENARIO("frank video run application 2", "[run_application_2]") {
  GIVEN("run the frank video application") {
    WHEN("no argument") {
      test::frank::mocked_list_input_devices_called = false;
      test::frank::mocked_opencv_ui_called = false;

      frank::video::run_application(0, nullptr);

      THEN("list_input_devices is called") {
        REQUIRE(test::frank::mocked_list_input_devices_called == true);
      }

      THEN("opencv_ui is called") {
        REQUIRE(test::frank::mocked_opencv_ui_called == true);
      }
    }
  }
}
