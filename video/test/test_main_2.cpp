#include "config.h"

#ifndef _TEST_MAIN_2_
#error _TEST_MAIN_2_ not defined
#endif

WARNINGS_OFF
#include <iostream>
WARNINGS_ON

#include <catch2/catch.hpp>

#include "run_application.h"

namespace test::frank {

class main_mock {
public:
  bool called{};
};

void mocked_run_application(int, char const *[], main_mock *mock) {
  if (!mock) {
    std::cerr << "mocked_run_application: no mock\n";
    return;
  }

  mock->called = true;
}

extern int main(int argc, char const *argv[], void *mock_data);

} // namespace test::frank

namespace frank::video {

void run_application(int argc, char const *argv[], void *mock_data) {
  if (!mock_data) {
    std::cerr << "run_application: no mock\n";
    test::frank::mocked_run_application(argc, argv, nullptr);
    return;
  }

  auto mock = static_cast<::test::frank::main_mock *>(mock_data);
  test::frank::mocked_run_application(argc, argv, mock);
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

      THEN("run_application is called") { REQUIRE(mock.called == true); }

      AND_THEN("main returns 1") { REQUIRE(results == ONE); }
    }
    WHEN("two arguments") {
      constexpr auto TWO = 2;
      char const *two_arguments[TWO]{TEST_APP_NAME, "first_argument"};
      test::frank::main_mock mock{};

      auto results = test::frank::main(TWO, two_arguments, &mock);

      THEN("run_application is called") { REQUIRE(mock.called == true); }

      AND_THEN("main returns 2") { REQUIRE(results == TWO); }
    }
  }
}
