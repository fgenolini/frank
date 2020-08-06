#include "config.h"

#ifndef _TEST_VIDEO_GUI_1_
#error _TEST_VIDEO_GUI_1_ not defined
#endif

WARNINGS_OFF
#include <catch2/catch.hpp>
WARNINGS_ON

#include "ui/video_gui.h"

SCENARIO("frank video GUI 1", "[video_gui_1]") {
  GIVEN("Video GUI") {
    WHEN("constructing with 1 webcam") {
      constexpr auto WEBCAM_COUNT = 1;
      auto make_video_gui = []() { frank::video::video_gui gui{WEBCAM_COUNT}; };

      THEN("no exception should be thrown") {
        REQUIRE_NOTHROW(make_video_gui());
      }
    }
  }
}
