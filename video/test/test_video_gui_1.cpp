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

class mock_cvui_init : public ::frank::video::cvui_init {
public:
  virtual void execute(const std::string[], size_t) override {
    cvui_init_called_ = true;
  }

  bool cvui_init_called() { return cvui_init_called_; }

private:
  bool cvui_init_called_{};
};

} // namespace test::frank

namespace frank::video {

cvui_init::~cvui_init() {}

void cvui_init::execute(const std::string[], size_t) {}

} // namespace frank::video

SCENARIO("frank video GUI 1", "[video_gui_1]") {
  GIVEN("Video GUI") {
    WHEN("constructing with 1 webcam") {
      constexpr auto WEBCAM_COUNT = 42;
      test::frank::mock_cvui_init mock{};

      auto make_video_gui = [&]() {
        frank::video::video_gui gui(WEBCAM_COUNT, &mock);
      };
      REQUIRE_NOTHROW(make_video_gui());

      THEN("cvui_init is called") { REQUIRE(mock.cvui_init_called() == true); }
    }
    WHEN("constructing using make_user_interface") {
      constexpr auto WEBCAM_COUNT = 0;
      test::frank::mock_cvui_init mock{};
      frank::video::user_interface_factory gui_factory{};

      auto make_video_gui = [&]() {
        auto gui = gui_factory.make(WEBCAM_COUNT, &mock);
      };
      REQUIRE_NOTHROW(make_video_gui());

      THEN("cvui_init is called") { REQUIRE(mock.cvui_init_called() == true); }
    }
  }
}
