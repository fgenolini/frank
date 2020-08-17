#include "config.h"

#ifndef _TEST_MAIN_2_
#error _TEST_MAIN_2_ not defined
#endif

WARNINGS_OFF
#include <catch2/catch.hpp>
WARNINGS_ON

#include "application.h"

namespace test::frank {

extern int main(int argc, char const *argv[], ::frank::video::application &app);

class mock_application : public ::frank::video::application {
public:
  void run(int, char const *[]) override { run_application_called_ = true; }

  bool run_application_called() { return run_application_called_; }

private:
  bool run_application_called_{};
};

} // namespace test::frank

namespace frank::video {

application::application(video_devices *devices, ui *ui_runner,
                         exceptions *exception_handler)
    : exception_handler_(exception_handler), ui_runner_(ui_runner),
      devices_(devices) {}

void application::run(int, char const *[]) {}

} // namespace frank::video

constexpr auto TEST_APP_NAME = "test_main_2";

SCENARIO("frank video main 2", "[main_2]") {
  GIVEN("main entry point for command line application") {
    WHEN("one argument") {
      constexpr auto ONE = 1;
      char const *one_argument[ONE]{TEST_APP_NAME};
      test::frank::mock_application mock{};

      auto results = test::frank::main(ONE, one_argument, mock);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called() == true);
      }

      THEN("main returns 1") { REQUIRE(results == ONE); }
    }
    WHEN("two arguments") {
      constexpr auto TWO = 2;
      char const *two_arguments[TWO]{TEST_APP_NAME, "first_argument"};
      test::frank::mock_application mock{};

      auto results = test::frank::main(TWO, two_arguments, mock);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called() == true);
      }

      THEN("main returns 2") { REQUIRE(results == TWO); }
    }
  }
}
