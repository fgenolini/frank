#include "config.h"

#ifndef _TEST_VIDEO_GUI_1_
#error _TEST_VIDEO_GUI_1_ not defined
#endif

WARNINGS_OFF
#include <iostream>
WARNINGS_ON

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

#include "ui/video_gui.h"

namespace test::frank {

} // namespace test::frank

namespace frank::video {

} // namespace frank::video

SCENARIO("frank video GUI 1", "[video_gui_1]") {
  GIVEN("Video GUI") {
    WHEN("constructing") {
      constexpr auto WEBCAM_COUNT = 1;
      frank::video::video_gui gui{WEBCAM_COUNT};

      THEN("exit is called") { REQUIRE(true == false); }
    }
  }
}
