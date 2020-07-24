#include <memory>

#include <opencv2/opencv.hpp>

#include "../device/input_device.h"
#include "main_window.h"
#include "../opencv/opencv_window.h"
#include "../opencv/take_picture.h"

namespace frank::video {

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled,
                 bool *overlay_enabled, double *overlay_alpha,
                 opencv_window &window) {
  auto first_time = window.first_time();
  auto high_threshold = window.high_threshold();
  auto low_threshold = window.low_threshold();
  auto main_frame = cv::Mat(200, 500, CV_8UC3);
  auto overlay_image = window.overlay_image();
  auto use_canny = window.use_canny();
  auto use_overlay = window.use_overlay();
  auto webcam_index = window.webcam_index();
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

  constexpr auto quit_x = 10;
  constexpr auto quit_y = 10;
  auto should_exit = cvui::button(main_frame, quit_x, quit_y, "Quit");
  if (should_exit) {
    window.set_exit_requested(true);
    return;
  }

  settings.begin(main_frame);
  if (!settings.isMinimized()) {
    cvui::checkbox("Use Canny Edge", &use_canny);
    cvui::beginRow();
    cvui::beginColumn();
    cvui::text(" ");
    cvui::text(" ");
    cvui::text("low:");
    cvui::endColumn();
    constexpr auto trackbar_width = 150;
    constexpr auto alpha_trackbar_width = 100;
    cvui::trackbar<int>(trackbar_width, &low_threshold, 5, 150);
    cvui::beginColumn();
    cvui::text(" ");
    cvui::text(" ");
    cvui::text("  high:");
    cvui::endColumn();
    cvui::trackbar<int>(trackbar_width, &high_threshold, 80, 300);
    cvui::endRow();
    for (auto webcam = 0; webcam < has_webcams.size(); ++webcam) {
      cvui::beginRow();
      cvui::beginColumn();
      std::string video_name{};
      if (input_devices.size() <= webcam) {
        video_name = "Video " + std::to_string(webcam);
      } else {
        video_name =
            std::to_string(webcam) + " " + input_devices[webcam].name();
      }

      cvui::text(" ");
      cvui::text(" ");
      cvui::checkbox(video_name, &video_enabled[webcam]);
      cvui::endColumn();
      cvui::beginColumn();
      cvui::text(" ");
      cvui::text(" ");
      cvui::endColumn();
      cvui::beginColumn();
      std::string overlay_name{"Ovl " + std::to_string(webcam)};
      cvui::text(" ");
      cvui::text(" ");
      cvui::checkbox(overlay_name, &overlay_enabled[webcam]);
      cvui::endColumn();
      constexpr auto alpha_max = 1.0;
      constexpr auto alpha_min = 0.0;
      constexpr auto label_format = "";
      cvui::trackbar<double>(alpha_trackbar_width, &overlay_alpha[webcam],
                             alpha_min, alpha_max);
      cvui::endRow();
    }
  }

  settings.end();
  cvui::imshow(window.name(), main_frame);
  window.set_use_canny(use_canny);
  for (auto webcam = 0; webcam < has_webcams.size(); ++webcam) {
    window.set_overlay_alpha(overlay_alpha[webcam]);
    window.set_use_overlay(overlay_enabled[webcam]);
  }

  window.set_low_threshold(low_threshold);
  window.set_high_threshold(high_threshold);
}

} // namespace frank::video
