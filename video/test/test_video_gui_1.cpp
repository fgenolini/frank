#include "config.h"

#ifndef _TEST_VIDEO_GUI_1_
#error _TEST_VIDEO_GUI_1_ not defined
#endif

WARNINGS_OFF
#include <iostream>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "ui/video_gui.h"

namespace test::frank {

class video_gui_mock {
public:
  bool cvui_init_called{};
};

void mock_cvui_init(const cv::String[], size_t, void *mock_data) {
  if (!mock_data) {
    std::cerr << "mock_cvui_init: no mock\n";
    return;
  }

  auto mock = static_cast<video_gui_mock *>(mock_data);
  mock->cvui_init_called = true;
}

} // namespace test::frank

SCENARIO("frank video GUI 1", "[video_gui_1]") {
  GIVEN("Video GUI") {
    WHEN("constructing with 1 webcam") {
      constexpr auto WEBCAM_COUNT = 1;
      test::frank::video_gui_mock mock{};

      auto make_video_gui = [&]() {
        frank::video::video_gui gui(WEBCAM_COUNT, test::frank::mock_cvui_init,
                                    &mock);
      };
      REQUIRE_NOTHROW(make_video_gui());

      THEN("mock_cvui_init is called") {
        REQUIRE(mock.cvui_init_called == true);
      }
    }
  }
}
