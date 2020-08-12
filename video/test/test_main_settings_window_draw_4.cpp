#include "config.h"

#ifndef _TEST_MAIN_SETTINGS_WINDOW_DRAW_4_
#error _TEST_MAIN_SETTINGS_WINDOW_DRAW_4_ not defined
#endif

WARNINGS_OFF
#include <iostream>
#include <vector>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/input_device.h"
#include "ui/main_settings_window.h"

namespace test::frank {

struct draw_mock {};

} // namespace test::frank

namespace frank::video {} // namespace frank::video

SCENARIO("frank video main settings window draw 4",
         "[main_settings_window_draw_4]") {
  GIVEN("main settings window minimised") {
    constexpr auto SETTINGS_MINIMISED = true;

    WHEN("draw") {
      std::vector<frank::video::input_device> devices{};
      std::vector<bool> has_webcams{};
      std::vector<cv::String> images{};
      frank::video::main_settings_window window(nullptr, nullptr, nullptr,
                                                nullptr, nullptr, nullptr);

      window.draw(SETTINGS_MINIMISED, devices, has_webcams, images);
    }
  }
}
