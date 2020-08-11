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
#include "ui/run_ui.h"

namespace test::frank {

struct run_application_mock {
  std::vector<::frank::video::input_device> list_devices() {
    list_devices_called = true;
    return std::vector<::frank::video::input_device>();
  }

  void run_ui() { run_ui_called = true; }

  bool list_devices_called{};
  bool run_ui_called{};
};

} // namespace test::frank

namespace frank::video {

std::vector<input_device> list_devices(void *mock_data) {
  if (!mock_data)
    return std::vector<input_device>();

  auto mock = static_cast<::test::frank::run_application_mock *>(mock_data);
  return mock->list_devices();
}

void run_ui(std::vector<input_device> const &, user_interface_factory,
            void *mock_data) {
  if (!mock_data)
    return;

  auto mock = static_cast<::test::frank::run_application_mock *>(mock_data);
  mock->run_ui();
}

} // namespace frank::video

SCENARIO("frank video run application 2", "[run_application_2]") {
  GIVEN("run the frank video application") {
    WHEN("no argument") {
      test::frank::run_application_mock mock{};

      frank::video::run_application(0, nullptr, &mock);

      THEN("list_input_devices is called") {
        REQUIRE(mock.list_devices_called == true);
      }

      THEN("run_ui is called") { REQUIRE(mock.run_ui_called == true); }
    }
  }
}
