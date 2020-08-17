#include "config.h"

#ifndef _TEST_WINMAIN_4_
#error _TEST_WINMAIN_4_ not defined
#endif

WARNINGS_OFF
#include <windows.h>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "run_application.h"

namespace test::frank {

struct winmain_mock {
  void run_application() { run_application_called = true; }

  bool run_application_called{};
};

extern int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option,
                            void *mock_data);

} // namespace test::frank

namespace frank::video {

run_application::run_application(void *mock_data) : mock_data_(mock_data) {}

void run_application::run(int, char const *[]) {
  auto mock = static_cast<::test::frank::winmain_mock *>(mock_data_);
  mock->run_application();
}

} // namespace frank::video

constexpr auto TEST_APP_NAME = "test_winmain_4";

SCENARIO("frank video winmain 4", "[winmain_4]") {
  GIVEN("WinMain entry point for Windows application") {
    WHEN("option show") {
      constexpr auto SHOW_OPTION = SW_SHOW;
      test::frank::winmain_mock mock{};

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION, &mock);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called == true);
      }

      THEN("WinMain returns SW_SHOW") { REQUIRE(results == SHOW_OPTION); }
    }
    WHEN("option minimise") {
      constexpr auto SHOW_OPTION = SW_MINIMIZE;
      test::frank::winmain_mock mock{};

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION, &mock);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called == true);
      }

      THEN("WinMain returns SW_MINIMIZE") { REQUIRE(results == SHOW_OPTION); }
    }
  }
}
