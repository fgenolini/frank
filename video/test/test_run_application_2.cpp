#include "config.h"

#ifndef _TEST_RUN_APPLICATION_2_
#error _TEST_RUN_APPLICATION_2_ not defined
#endif

WARNINGS_OFF
#include <iostream>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/list_devices.h"
#include "exception/handler.h"
#include "run_application.h"
#include "ui/run_ui.h"

namespace test::frank {

struct run_application_exception : public std::exception {};

class run_application_mock {
public:
  run_application_mock(bool throw_in_list_devices = false)
      : throw_in_list_devices_(throw_in_list_devices) {}

  std::vector<::frank::video::input_device> list_devices() {
    list_devices_called = true;
    if (throw_in_list_devices_)
      throw exception_;

    return std::vector<::frank::video::input_device>();
  }

  void run_ui() { run_ui_called = true; }

  bool list_devices_called{};
  bool run_ui_called{};

private:
  bool throw_in_list_devices_{};
  run_application_exception exception_{};
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

static auto exceptions_handler_called = false;

void all_exceptions_handler() {
  std::cerr << "Mocked unhandled exception\n";
  exceptions_handler_called = true;
}

} // namespace frank::video

SCENARIO("frank video run application 2", "[run_application_2]") {
  GIVEN("run the frank video application") {
    WHEN("no argument") {
      frank::video::exceptions_handler_called = false;
      test::frank::run_application_mock mock{};

      frank::video::run_application(0, nullptr, &mock);

      THEN("list_devices is called") {
        REQUIRE(mock.list_devices_called == true);
      }

      THEN("run_ui is called") { REQUIRE(mock.run_ui_called == true); }

      THEN("exceptions_handler is not called") {
        REQUIRE(frank::video::exceptions_handler_called == false);
      }
    }
    WHEN("exception thrown in list_devices") {
      constexpr auto THROW_IN_LIST_DEVICES = true;
      frank::video::exceptions_handler_called = false;
      test::frank::run_application_mock mock{THROW_IN_LIST_DEVICES};

      frank::video::run_application(0, nullptr, &mock);

      THEN("list_devices is called") {
        REQUIRE(mock.list_devices_called == true);
      }

      THEN("exceptions_handler is called") {
        REQUIRE(frank::video::exceptions_handler_called == true);
      }
    }
  }
}
