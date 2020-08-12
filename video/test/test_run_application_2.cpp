#include "config.h"

#ifndef _TEST_RUN_APPLICATION_2_
#error _TEST_RUN_APPLICATION_2_ not defined
#endif

WARNINGS_OFF
#include <exception>
#include <iostream>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/list_devices.h"
#include "exception/handler.h"
#include "run_application.h"
#include "ui/run_ui.h"

namespace test::frank {

struct run_application_exception : public std::exception {
  char const *what() const noexcept override {
    return "run_application_2 exception";
  }
};

enum class exception_type {
  none,
  exception_object,
  int_number,
};

constexpr auto INT_EXCEPTION = 42;

WARNING_PUSH
DISABLE_WARNING_MSC(4820)
class run_application_mock {
public:
  run_application_mock(
      exception_type throw_in_list_devices = exception_type::none)
      : throw_in_list_devices_(throw_in_list_devices) {}

  std::exception const *exception_caught() const { return caught_exception_; }

  void handler(std::exception const *caught_exception = nullptr) {
    exceptions_handler_called = true;
    caught_exception_ = caught_exception;
  }

  std::vector<::frank::video::input_device> list_devices() {
    list_devices_called = true;
    if (throw_in_list_devices_ == exception_type::exception_object)
      throw exception_;

    if (throw_in_list_devices_ == exception_type::int_number)
      throw INT_EXCEPTION;

    return std::vector<::frank::video::input_device>();
  }

  void run_ui() { run_ui_called = true; }

private:
  std::exception const *caught_exception_{};
  run_application_exception exception_{};
  exception_type throw_in_list_devices_{};

public:
  bool exceptions_handler_called{};
  bool list_devices_called{};
  bool run_ui_called{};
};
WARNINGS_ON

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

void all_exceptions_handler(std::exception const *caught_exception,
                            void *mock_data) noexcept {
  std::cerr << "Mocked exception handler\n";
  if (!mock_data)
    return;

  auto mock = static_cast<::test::frank::run_application_mock *>(mock_data);
  mock->handler(caught_exception);
}

void unhandled_exception_handler() noexcept {
  all_exceptions_handler(nullptr, nullptr);
}

} // namespace frank::video

SCENARIO("frank video run application 2", "[run_application_2]") {
  GIVEN("run the frank video application") {
    WHEN("no argument") {
      test::frank::run_application_mock mock{};

      frank::video::run_application(0, nullptr, &mock);

      THEN("list_devices is called") {
        REQUIRE(mock.list_devices_called == true);
      }

      THEN("run_ui is called") { REQUIRE(mock.run_ui_called == true); }

      THEN("exceptions_handler is not called") {
        REQUIRE(mock.exceptions_handler_called == false);
      }
    }
    WHEN("exception object thrown in list_devices") {
      constexpr auto THROW_IN_LIST_DEVICES =
          test::frank::exception_type::exception_object;
      test::frank::run_application_mock mock{THROW_IN_LIST_DEVICES};

      frank::video::run_application(0, nullptr, &mock);

      THEN("list_devices is called") {
        REQUIRE(mock.list_devices_called == true);
      }

      THEN("exceptions_handler is called") {
        REQUIRE(mock.exceptions_handler_called == true);
      }
    }
    WHEN("int number thrown in list_devices") {
      constexpr auto THROW_IN_LIST_DEVICES =
          test::frank::exception_type::int_number;
      test::frank::run_application_mock mock{THROW_IN_LIST_DEVICES};

      frank::video::run_application(0, nullptr, &mock);

      THEN("list_devices is called") {
        REQUIRE(mock.list_devices_called == true);
      }

      THEN("exceptions_handler is called") {
        REQUIRE(mock.exceptions_handler_called == true);
      }
    }
  }
}
