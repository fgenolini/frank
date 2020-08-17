#include "config.h"

#ifndef _TEST_MAIN_2_
#error _TEST_MAIN_2_ not defined
#endif

WARNINGS_OFF
#include <catch2/catch.hpp>
WARNINGS_ON

#include "application.h"

namespace test::frank {

struct main_mock {
  void run_application() { run_application_called = true; }

  bool run_application_called{};
};

extern int main(int argc, char const *argv[], void *mock_data);

} // namespace test::frank

namespace frank::video {

application::application(void *mock_data) : mock_data_(mock_data) {}

void application::run(int, char const *[]) {
  auto mock = static_cast<::test::frank::main_mock *>(mock_data_);
  mock->run_application();
}

} // namespace frank::video

constexpr auto TEST_APP_NAME = "test_main_2";

SCENARIO("frank video main 2", "[main_2]") {
  GIVEN("main entry point for command line application") {
    WHEN("one argument") {
      constexpr auto ONE = 1;
      char const *one_argument[ONE]{TEST_APP_NAME};
      test::frank::main_mock mock{};

      auto results = test::frank::main(ONE, one_argument, &mock);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called == true);
      }

      THEN("main returns 1") { REQUIRE(results == ONE); }
    }
    WHEN("two arguments") {
      constexpr auto TWO = 2;
      char const *two_arguments[TWO]{TEST_APP_NAME, "first_argument"};
      test::frank::main_mock mock{};

      auto results = test::frank::main(TWO, two_arguments, &mock);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called == true);
      }

      THEN("main returns 2") { REQUIRE(results == TWO); }
    }
  }
}
