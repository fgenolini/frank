#include "config.h"

#ifndef _TEST_VIDEO_GUI_1_
#error _TEST_VIDEO_GUI_1_ not defined
#endif

WARNINGS_OFF
#include <catch2/catch.hpp>
WARNINGS_ON

#include "ui/cvui_init.h"
#include "ui/video_gui.h"

namespace test::frank {

struct video_gui_mock {
  bool cvui_init_called{};
};

} // namespace test::frank

namespace frank::video {

void cvui_init::execute(const std::string[], size_t, void *mock_data) const {
  auto mock = static_cast<test::frank::video_gui_mock *>(mock_data);
  mock->cvui_init_called = true;
}

} // namespace frank::video

SCENARIO("frank video GUI 1", "[video_gui_1]") {
  GIVEN("Video GUI") {
    WHEN("constructing with 1 webcam") {
      constexpr auto WEBCAM_COUNT = 42;
      frank::video::cvui_init mock_cvui_init{};
      test::frank::video_gui_mock mock{};

      auto make_video_gui = [&]() {
        frank::video::video_gui gui(WEBCAM_COUNT, mock_cvui_init, &mock);
      };
      REQUIRE_NOTHROW(make_video_gui());

      THEN("cvui_init is called") { REQUIRE(mock.cvui_init_called == true); }
    }
    WHEN("constructing using make_user_interface") {
      constexpr auto WEBCAM_COUNT = 0;
      frank::video::cvui_init mock_cvui_init{};
      test::frank::video_gui_mock mock{};

      auto make_video_gui = [&]() {
        auto gui = frank::video::make_user_interface(WEBCAM_COUNT,
                                                     mock_cvui_init, &mock);
      };
      REQUIRE_NOTHROW(make_video_gui());

      THEN("cvui_init is called") { REQUIRE(mock.cvui_init_called == true); }
    }
  }
}
