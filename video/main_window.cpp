#include <cstdlib>
#include <memory>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "input_device.h"
#include "main_window.h"
#include "opencv_window.h"
#include "take_picture.h"

namespace frank::video {

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled,
                 opencv_window &window) {
  auto first_time = window.first_time();
  auto webcam_index = window.webcam_index();
  auto use_canny = window.use_canny();
  auto low_threshold = window.low_threshold();
  auto high_threshold = window.high_threshold();
  auto main_frame = cv::Mat(200, 500, CV_8UC3);
  main_frame = cv::Scalar(49, 52, 49);
  cvui::context(window.name());
  if (!first_time && has_webcams[0] && video_enabled[0]) {
    auto picture = take_picture(window);
    if (window.exit_requested()) {
      return;
    }

    if (use_canny) {
      cv::cvtColor(picture, main_frame, cv::COLOR_BGR2GRAY);
      cv::Canny(main_frame, main_frame, low_threshold, high_threshold, 3);
      cv::cvtColor(main_frame, main_frame, cv::COLOR_GRAY2BGR);
    } else {
      main_frame = picture;
    }
  }

  if (first_time && has_webcams[0]) {
    cvui::printf(main_frame, 10, 10, "Opening webcam %d...", webcam_index);
  }

  auto should_exit = cvui::button(main_frame, 110, 80, "Quit");
  if (should_exit) {
    window.set_exit_requested(true);
    return;
  }

  settings.begin(main_frame);
  if (!settings.isMinimized()) {
    cvui::checkbox("Use Canny Edge", &use_canny);
    cvui::trackbar(165, &low_threshold, 5, 150);
    cvui::trackbar(165, &high_threshold, 80, 300);
    for (auto i = 0; i < has_webcams.size(); ++i) {
      std::string video_name{};
      if (input_devices.size() <= i) {
        video_name = "Video " + std::to_string(i);
      } else {
        video_name = std::to_string(i) + " " + input_devices[i].name();
      }

      cvui::checkbox(video_name, &video_enabled[i]);
    }
  }

  settings.end();
  cvui::imshow(window.name(), main_frame);
  window.set_use_canny(use_canny);
  window.set_low_threshold(low_threshold);
  window.set_high_threshold(high_threshold);
};

} // namespace frank::video
