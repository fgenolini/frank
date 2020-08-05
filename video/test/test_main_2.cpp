#include "config.h"

#ifndef _TEST_MAIN_2_
#error _TEST_MAIN_2_ not defined
#endif

#include <catch2/catch.hpp>

#include "run_application.h"

namespace test::frank {

static auto mock_called = false;

void mocked_run_application(int argc, char const *argv[]) {
  mock_called = true;
}

extern int main(int argc, char const *argv[]);

} // namespace test::frank

namespace frank::video {

void run_application(int argc, char const *argv[]) {
  test::frank::mocked_run_application(argc, argv);
}

} // namespace frank::video

constexpr auto TEST_APP_NAME = "test_main_2";

SCENARIO("frank video main 2", "[test_main_2]") {
  GIVEN("main entry point for command line application") {
    WHEN("one argument") {
      test::frank::mock_called = false;
      constexpr auto ONE = 1;
      char const *one_argument[ONE]{TEST_APP_NAME};

      auto results = test::frank::main(ONE, one_argument);

      THEN("run_application is called") {
        REQUIRE(test::frank::mock_called == true);
      }

      AND_THEN("main returns 1") { REQUIRE(results == ONE); }
    }
    WHEN("two arguments") {
      test::frank::mock_called = false;
      constexpr auto TWO = 2;
      char const *two_arguments[TWO]{TEST_APP_NAME, "first_argument"};

      auto results = test::frank::main(TWO, two_arguments);

      THEN("run_application is called") {
        REQUIRE(test::frank::mock_called == true);
      }

      AND_THEN("main returns 2") { REQUIRE(results == TWO); }
    }
  }
}
