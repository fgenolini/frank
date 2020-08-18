#include "config.h"

#ifndef _TEST_EXCEPTIONS_HANDLER_
#error _TEST_EXCEPTIONS_HANDLER_ not defined
#endif

WARNINGS_OFF
#include <exception>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "exception/exceptions.h"
#include "test/testable_exit.h"

namespace test::frank {

struct test_exception : public std::exception {
  char const *what() const noexcept override {
    return "exceptions_handler exception";
  }
};

class mock_aborter {
public:
  void abort() noexcept { abort_called_ = true; }

  bool abort_called() { return abort_called_; }

private:
  bool abort_called_{};
};

} // namespace test::frank

namespace frank::video {

aborter::aborter(void *mock_data) : mock_data_(mock_data) {}

void aborter::abort() noexcept {
  auto mock = static_cast<::test::frank::mock_aborter *>(mock_data_);
  mock->abort();
}

} // namespace frank::video

SCENARIO("frank video exceptions_handler", "[exceptions_handler]") {
  GIVEN("exception thrown from the frank video application") {
    WHEN("no exception thrown") {
      test::frank::test_exception exception_object{};
      test::frank::mock_aborter mocked_aborter{};
      frank::video::aborter mock{&mocked_aborter};
      frank::video::global_injected_aborter = nullptr;
      frank::video::exceptions exception_test{&mock};

      try {
        // do nothing
      } catch (...) {
        exception_test.handler();
      }

      THEN("abort is not called") {
        REQUIRE(mocked_aborter.abort_called() == false);
      }
    }
    WHEN("exception object thrown") {
      test::frank::test_exception exception_object{};
      test::frank::mock_aborter mocked_aborter{};
      frank::video::aborter mock{&mocked_aborter};
      frank::video::global_injected_aborter = nullptr;
      frank::video::exceptions exception_test{&mock};

      try {
        throw exception_object;
      } catch (const std::exception &e) {
        exception_test.handler(&e);
      }

      THEN("abort is called") {
        REQUIRE(mocked_aborter.abort_called() == true);
      }
    }
    WHEN("int number thrown") {
      constexpr auto INT_EXCEPTION = 42;
      test::frank::mock_aborter mocked_aborter{};
      frank::video::aborter mock{&mocked_aborter};
      frank::video::global_injected_aborter = nullptr;
      frank::video::exceptions exception_test{&mock};

      try {
        throw INT_EXCEPTION;
      } catch (...) {
        exception_test.handler();
      }

      THEN("abort is called") {
        REQUIRE(mocked_aborter.abort_called() == true);
      }
    }
    WHEN("exception thrown") {
      test::frank::mock_aborter mocked_aborter{};
      frank::video::aborter mock{&mocked_aborter};
      frank::video::global_injected_aborter = &mock;

      try {
        throw nullptr;
      } catch (...) {
        frank::video::unhandled_exception_handler();
      }

      THEN("abort is called") {
        REQUIRE(mocked_aborter.abort_called() == true);
      }
    }
  }
}
