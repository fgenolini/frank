#include <opencv2/opencv.hpp>

#include "cvui.h"

#include "opencv_window.h"
#include "other_window.h"
#include "take_picture.h"

namespace frank::video {

void other_window(opencv_window &window) {
  auto first_time = window.first_time();
  auto use_canny = window.use_canny();
  auto low_threshold = window.low_threshold();
  auto high_threshold = window.high_threshold();
  auto other_frame = cv::Mat(200, 500, CV_8UC3);
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
