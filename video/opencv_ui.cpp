#include <cstdlib>

#include <opencv2/opencv.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "EnhancedWindow.h"

#include "opencv_ui.h"

namespace frank::video {

[[noreturn]] void opencv_ui() {
  constexpr auto ESCAPE_KEY = 27;
  constexpr auto SHORT_DELAY = 20;
  constexpr auto VIDEO_COUNT = 4;
  constexpr auto WINDOW_NAME = "Frank video";

  bool use_video[VIDEO_COUNT]{};
  auto frame = cv::Mat(200, 500, CV_8UC3);
  EnhancedWindow settings(200, 50, 200, 100, "Settings");

  cvui::init(WINDOW_NAME);
  while (true) {
    frame = cv::Scalar(49, 52, 49);
    auto button_clicked = cvui::button(frame, 110, 80, "Quit");
    if (button_clicked) {
      exit(EXIT_SUCCESS);
    }

    settings.begin(frame);
    if (!settings.isMinimized()) {
      for (auto i = 0; i < VIDEO_COUNT; ++i) {
        std::string video_name{"Video " + std::to_string(i)};
        cvui::checkbox(video_name, &use_video[i]);
      }
    }

    settings.end();

    cvui::imshow(WINDOW_NAME, frame);
    if (cv::waitKey(SHORT_DELAY) == ESCAPE_KEY) {
      exit(EXIT_SUCCESS);
    }
  }

  exit(EXIT_SUCCESS);
}

} // namespace frank::video
