#include <opencv2/opencv.hpp>

#define CVUI_DISABLE_COMPILATION_NOTICES
#include "cvui.h"

#include "../opencv/opencv_window.h"
#include "../opencv/take_picture.h"
#include "other_window.h"

namespace frank::video {

void other_window(opencv_window &window) {
  constexpr auto WINDOW_HEIGHT = 180;
  constexpr auto WINDOW_WIDTH = 320;
  auto const first_time = window.first_time();
  auto const high_threshold = window.high_threshold();
  auto const low_threshold = window.low_threshold();
  auto const overlay_alpha = window.overlay_alpha();
  auto const overlay_image = window.overlay_image();
  auto const use_canny = window.use_canny();
  auto const use_overlay = window.use_overlay();
  auto other_frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  other_frame = cv::Scalar(49, 52, 49);
  cvui::context(window.name());
  if (!first_time && window.has_webcam() && window.video_enabled()) {
    auto picture = take_picture(window);
    if (window.exit_requested()) {
      return;
    }

    if (use_canny) {
      cv::cvtColor(picture, other_frame, cv::COLOR_BGR2GRAY);
      cv::Canny(other_frame, other_frame, low_threshold, high_threshold, 3);
      cv::cvtColor(other_frame, other_frame, cv::COLOR_GRAY2BGR);
    } else {
      other_frame = picture;
    }
  }

  if (first_time && window.has_webcam()) {
    cvui::printf(other_frame, 10, 10, "Opening webcam %d...",
                 window.webcam_index());
  }

  cvui::imshow(window.name(), other_frame);
}

} // namespace frank::video
