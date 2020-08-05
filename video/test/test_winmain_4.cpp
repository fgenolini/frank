#include "config.h"

#ifndef _TEST_WINMAIN_4_
#error _TEST_WINMAIN_4_ not defined
#endif

WARNINGS_OFF
#include <iostream>

#include <windows.h>
WARNINGS_ON

#include <catch2/catch.hpp>

#include "run_application.h"

namespace test::frank {

class winmain_mock {
public:
  bool called{};
};

void mocked_run_application(winmain_mock *mock) {
  if (!mock) {
    std::cerr << "mocked_run_application: no mock\n";
    return;
  }

  mock->called = true;
}

extern int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option),
    void *mock_data;

} // namespace test::frank

namespace frank::video {

void run_application(int, char const *[], void *mock_data) {
  if (!mock_data) {
    std::cerr << "run_application: no mock\n";
    test::frank::mocked_run_application(nullptr);
    return;
  }

  auto mock = static_cast<::test::frank::winmain_mock *>(mock_data);
  test::frank::mocked_run_application(mock);
}

} // namespace frank::video

constexpr auto TEST_APP_NAME = "test_winmain_4";

SCENARIO("frank video winmain 4", "[test_winmain_4]") {
  GIVEN("WinMain entry point for Windows application") {
    WHEN("option show") {
      constexpr auto SHOW_OPTION = SW_SHOW;
      test::frank::winmain_mock mock{};

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION, &mock);

      THEN("run_application is called") { REQUIRE(mock.called == true); }

      AND_THEN("WinMain returns SW_SHOW") { REQUIRE(results == SHOW_OPTION); }
    }
    WHEN("option minimise") {
      constexpr auto SHOW_OPTION = SW_MINIMIZE;
      test::frank::winmain_mock mock{};

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION, &mock);

      THEN("run_application is called") { REQUIRE(mock.called == true); }

      AND_THEN("WinMain returns SW_MINIMIZE") {
        REQUIRE(results == SHOW_OPTION);
      }
    }
  }
}
