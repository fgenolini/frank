#include <filesystem>

#include <opencv2/opencv.hpp>

#include "../device/input_device.h"
#include "../opencv/opencv_window.h"
#include "../opencv/take_picture.h"
#include "main_settings_window.h"
#include "main_window.h"

namespace fs = std::filesystem;

namespace frank::video {

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled_array,
                 bool *overlay_enabled_array, double *overlay_alpha_array,
                 std::vector<cv::String> &overlay_images,
                 opencv_window &window) {
  constexpr auto QUIT_X = 10;
  constexpr auto QUIT_Y = 10;
  constexpr auto WINDOW_HEIGHT = 480;
  constexpr auto WINDOW_WIDTH = 640;
  auto const first_time = window.first_time();
  auto const webcam_index = window.webcam_index();
  auto high_threshold = window.high_threshold();
  auto low_threshold = window.low_threshold();
  auto main_frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  auto use_canny = window.use_canny();
  main_frame = cv::Scalar(49, 52, 49);
  cvui::context(window.name());
  if (!first_time && has_webcams[0] && video_enabled_array[0]) {
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

  auto should_exit = cvui::button(main_frame, QUIT_X, QUIT_Y, "Quit");
  if (should_exit) {
    window.set_exit_requested(true);
    return;
  }

  settings.begin(main_frame);
  main_settings_window settings_window{
      video_enabled_array,   &use_canny,
      &low_threshold,        &high_threshold,
      overlay_enabled_array, overlay_alpha_array};
  settings_window.draw(settings, input_devices, has_webcams, overlay_images);
  settings.end();
  cvui::imshow(window.name(), main_frame);
  window.set_use_canny(use_canny);
  window.set_low_threshold(low_threshold);
  window.set_high_threshold(high_threshold);
}

} // namespace frank::video
