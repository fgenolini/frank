#include <array>
#include <cstdlib>
#include <memory>
#include <utility>

#include <gsl/gsl_util>

using namespace gsl;

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#define CVUI_IMPLEMENTATION

#include "../device/input_device.h"
#include "../opencv/exit_requested.h"
#include "../opencv/opencv_window.h"
#include "../opencv/take_picture.h"
#include "main_window.h"
#include "opencv_ui.h"
#include "other_window.h"

namespace frank::video {

bool opencv_with_webcams(std::vector<input_device> &connected_webcams) {
  constexpr auto MAXIMUM_VIDEO_COUNT = 4;
  constexpr auto WINDOW_NAME = "Frank video";
  constexpr auto WINDOW1_NAME = "Frank video 1";
  constexpr auto WINDOW2_NAME = "Frank video 2";
  constexpr auto WINDOW3_NAME = "Frank video 3";

  std::vector<cv::String> overlay_images{};
  std::vector<cv::String> window_names = {WINDOW_NAME, WINDOW1_NAME,
                                          WINDOW2_NAME, WINDOW3_NAME};
  std::vector<std::pair<double, double>> height_width_pairs{};
  double overlay_alpha[MAXIMUM_VIDEO_COUNT]{};
  bool overlay_enabled[MAXIMUM_VIDEO_COUNT]{};
  bool video_enabled[MAXIMUM_VIDEO_COUNT]{};
  std::vector<bool> has_webcams{};
  for (auto webcam = 0;
       webcam < (int)window_names.size() && webcam < MAXIMUM_VIDEO_COUNT;
       ++webcam) {
    auto has_webcam = (int)connected_webcams.size() > webcam;
    has_webcams.push_back(has_webcam);
    video_enabled[webcam] = has_webcam;
  }

  if (!has_webcams[0]) {
    // Index 0 for OpenCV is also any default video input device, assume there
    // is at least one
    has_webcams[0] = true;
  }

  std::vector<std::unique_ptr<cv::VideoCapture>> input_video_devices{};
  for ([[maybe_unused]] auto _ : has_webcams) {
    auto input_video_device = std::make_unique<cv::VideoCapture>();
    input_video_devices.push_back(std::move(input_video_device));
    overlay_images.push_back("");
    height_width_pairs.push_back(std::make_pair(0.0, 0.0));
  }

  auto _ = finally([&input_video_devices] {
    std::cout << "Releasing webcams" << '\n';
    for (auto &video_capture : input_video_devices) {
      auto capturing_device = video_capture.get();
      capturing_device->release();
    }
  });

  constexpr auto FIRST_TIME = true;
  constexpr auto HIGH_THRESHOLD = 150;
  constexpr auto LOW_THRESHOLD = 50;
  constexpr auto NO_OVERLAY_ALPHA = 0.0;
  constexpr auto USE_CANNY = false;
  constexpr auto USE_OVERLAY = false;
  constexpr auto WEBCAM_INDEX = 0;
  cv::String overlay_image{};
  opencv_window window_template(
      window_names[0], input_video_devices[0].get(), WEBCAM_INDEX, FIRST_TIME,
      has_webcams[0], video_enabled[0], std::make_pair(0.0, 0.0), USE_CANNY,
      USE_OVERLAY, overlay_image, NO_OVERLAY_ALPHA, LOW_THRESHOLD,
      HIGH_THRESHOLD);
  constexpr auto SETTINGS_HEIGHT = 280;
  constexpr auto SETTINGS_WIDTH = 600;
  constexpr auto SETTINGS_X = 10;
  constexpr auto SETTINGS_Y = 50;
  EnhancedWindow settings(SETTINGS_X, SETTINGS_Y, SETTINGS_WIDTH,
                          SETTINGS_HEIGHT, "Settings");
  cvui::init(&window_names[0], window_names.size());
  while (true) {
    main_window(settings, connected_webcams, has_webcams, video_enabled,
                overlay_enabled, overlay_alpha, overlay_images,
                window_template);
    if (window_template.exit_requested()) {
      return true;
    }

    for (auto webcam = 1; webcam < (int)window_names.size(); ++webcam) {
      if (!has_webcams[webcam]) {
        continue;
      }

      auto height_width_pair = height_width_pairs[webcam];
      opencv_window window(
          window_names[webcam], input_video_devices[webcam].get(), webcam,
          window_template.first_time(), has_webcams[webcam],
          video_enabled[webcam], height_width_pair, window_template.use_canny(),
          window_template.use_overlay(), overlay_images[webcam],
          overlay_alpha[webcam], window_template.low_threshold(),
          window_template.high_threshold());
      other_window(window);
      if (window.exit_requested()) {
        return true;
      }

      height_width_pair.first = window.height();
      height_width_pair.second = window.width();
      height_width_pairs[webcam] = height_width_pair;
    }

    if (exit_requested()) {
      return true;
    }

    window_template.set_first_time(false);
  }

  return false;
}

[[noreturn]] void opencv_ui(std::vector<input_device> &connected_webcams) {
  if (opencv_with_webcams(connected_webcams)) {
    exit(EXIT_SUCCESS);
  } else {
    exit(EXIT_FAILURE);
  }
}

} // namespace frank::video
