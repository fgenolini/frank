#include "config.h"

#ifndef _TEST_EXCEPTIONS_HANDLER_
#error _TEST_EXCEPTIONS_HANDLER_ not defined
#endif

WARNINGS_OFF
#include <exception>
#include <iostream>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "exception/exceptions_handler.h"
#include "test/testable_exit.h"

namespace test::frank {

struct test_exception : public std::exception {
  char const *what() const noexcept override {
    return "exceptions_handler exception";
  }
};

WARNING_PUSH
DISABLE_WARNING_MSC(4820)
struct exceptions_handler_mock {
  void abort() noexcept { abort_called = true; }

  bool abort_called{};
};
WARNINGS_ON

} // namespace test::frank

namespace frank::video {

static auto abort_called = false;

void abort(void *mock_data) noexcept {
  abort_called = true;
  if (!mock_data)
    return;
  auto mock = static_cast<::test::frank::exceptions_handler_mock *>(mock_data);
  return mock->abort();
}

} // namespace frank::video

SCENARIO("frank video exceptions_handler", "[exceptions_handler]") {
  GIVEN("exception thrown from the frank video application") {
    WHEN("exception object thrown") {
      test::frank::test_exception exception_object{};
      test::frank::exceptions_handler_mock mock{};

      try {
        throw exception_object;
      } catch (const std::exception &e) {
        frank::video::exceptions_handler(&e, &mock);
      }

      THEN("abort is called") { REQUIRE(mock.abort_called == true); }
    }
    WHEN("int number thrown") {
      constexpr auto INT_EXCEPTION = 42;
      test::frank::exceptions_handler_mock mock{};

      try {
        throw INT_EXCEPTION;
      } catch (...) {
        frank::video::exceptions_handler(nullptr, &mock);
      }

      THEN("abort is called") { REQUIRE(mock.abort_called == true); }
    }
    WHEN("exception thrown") {
      frank::video::abort_called = false;
      try {
        throw nullptr;
      } catch (...) {
        frank::video::unhandled_exception_handler();
      }

      THEN("abort is called") { REQUIRE(frank::video::abort_called == true); }
    }
  }
}
