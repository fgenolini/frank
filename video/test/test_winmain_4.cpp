#include "config.h"

#ifndef _TEST_WINMAIN_4_
#error _TEST_WINMAIN_4_ not defined
#endif

WARNINGS_OFF
#include <windows.h>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "application.h"

namespace test::frank {

extern int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option,
                            ::frank::video::application &app);

class mock_application {
public:
  void run(int, char const *[]) { run_application_called_ = true; }

  bool run_application_called() { return run_application_called_; }

private:
  bool run_application_called_{};
};

} // namespace test::frank

namespace frank::video {

application::application(video_devices *devices, ui *ui_runner,
                         exceptions *exception_handler, void *mock_data)
    : exception_handler_(exception_handler), ui_runner_(ui_runner),
      devices_(devices), mock_data_(mock_data) {}

void application::run(int argc, char const *argv[]) {
  auto mock = static_cast<::test::frank::mock_application *>(mock_data_);
  mock->run(argc, argv);
}

} // namespace frank::video

constexpr auto TEST_APP_NAME = "test_winmain_4";

SCENARIO("frank video winmain 4", "[winmain_4]") {
  GIVEN("WinMain entry point for Windows application") {
    WHEN("option show") {
      constexpr auto SHOW_OPTION = SW_SHOW;
      test::frank::mock_application mock{};
      frank::video::application app(nullptr, nullptr, nullptr, &mock);

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION, app);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called() == true);
      }

      THEN("WinMain returns SW_SHOW") { REQUIRE(results == SHOW_OPTION); }
    }
    WHEN("option minimise") {
      constexpr auto SHOW_OPTION = SW_MINIMIZE;
      test::frank::mock_application mock{};
      frank::video::application app(nullptr, nullptr, nullptr, &mock);

      auto results =
          test::frank::WinMain(nullptr, nullptr, nullptr, SHOW_OPTION, app);

      THEN("run_application is called") {
        REQUIRE(mock.run_application_called() == true);
      }

      THEN("WinMain returns SW_MINIMIZE") { REQUIRE(results == SHOW_OPTION); }
    }
  }
}
