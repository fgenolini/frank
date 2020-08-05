#include "config.h"

#include <windows.h>

#ifndef _TEST_WINMAIN_4_
#error _TEST_WINMAIN_4_ not defined
#endif

#include <catch2/catch.hpp>

#include "run_application.h"

namespace test::frank {

static auto mock_called = false;

void mocked_run_application() { mock_called = true; }

extern int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option);

} // namespace test::frank

namespace frank::video {

void run_application(int, char const *[]) {
  test::frank::mocked_run_application();
}

} // namespace frank::video

constexpr auto TEST_APP_NAME = "test_winmain_4";

SCENARIO("frank video winmain 4", "[test_winmain_4]") {
  GIVEN("WinMain entry point for Windows application") {
    WHEN("option show") {
      test::frank::mock_called = false;
      constexpr auto SHOW_OPTION = SW_SHOW;

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION);

      THEN("run_application is called") {
        REQUIRE(test::frank::mock_called == true);
      }

      AND_THEN("WinMain returns SW_SHOW") { REQUIRE(results == SHOW_OPTION); }
    }
    WHEN("option minimise") {
      test::frank::mock_called = false;
      constexpr auto SHOW_OPTION = SW_MINIMIZE;

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION);

      THEN("run_application is called") {
        REQUIRE(test::frank::mock_called == true);
      }

      AND_THEN("WinMain returns SW_MINIMIZE") {
        REQUIRE(results == SHOW_OPTION);
      }
    }
  }
}
