#include <array>
#include <cstdlib>
#include <memory>

#include <gsl/gsl_util>

using namespace gsl;

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#define CVUI_IMPLEMENTATION

#include "../opencv/exit_requested.h"
#include "../input_device.h"
#include "main_window.h"
#include "opencv_ui.h"
#include "../opencv/opencv_window.h"
#include "other_window.h"
#include "../opencv/take_picture.h"

namespace frank::video {

bool opencv_with_webcams(std::vector<input_device> &connected_webcams) {
  constexpr auto MAXIMUM_VIDEO_COUNT = 4;
  constexpr auto WINDOW_NAME = "Frank video";
  constexpr auto WINDOW1_NAME = "Frank video 1";
  constexpr auto WINDOW2_NAME = "Frank video 2";
  constexpr auto WINDOW3_NAME = "Frank video 3";

  std::vector<cv::String> window_names = {WINDOW_NAME, WINDOW1_NAME,
                                          WINDOW2_NAME, WINDOW3_NAME};
  bool video_enabled[MAXIMUM_VIDEO_COUNT]{};
  std::vector<bool> has_webcams{};
  for (auto i = 0; i < window_names.size() && i < MAXIMUM_VIDEO_COUNT; ++i) {
    auto has_webcam = connected_webcams.size() > i;
    has_webcams.push_back(has_webcam);
    video_enabled[i] = has_webcam;
  }

  if (!has_webcams[0]) {
    // Index 0 for OpenCV is also any default video input device, assume there
    // is at least one
    has_webcams[0] = true;
  }

  std::vector<std::unique_ptr<cv::VideoCapture>> input_video_devices{};
  for (auto i = 0; i < has_webcams.size(); ++i) {
    auto input_video_device = std::make_unique<cv::VideoCapture>();
    input_video_devices.push_back(std::move(input_video_device));
  }

  auto _ = finally([&input_video_devices] {
    std::cout << "Releasing webcams" << '\n';
    for (auto &video_capture : input_video_devices) {
      auto capturing_device = video_capture.get();
      capturing_device->release();
    }
  });

  constexpr auto webcam_index = 0;
  constexpr auto first_time = true;
  constexpr auto use_canny = false;
  opencv_window window_template(window_names[0], input_video_devices[0].get(),
                                webcam_index, first_time, has_webcams[0],
                                video_enabled[0], use_canny, 50, 150);
  EnhancedWindow settings(200, 50, 250, 250, "Settings");
  cvui::init(&window_names[0], window_names.size());
  while (true) {
    main_window(settings, connected_webcams, has_webcams, video_enabled,
                window_template);
    if (window_template.exit_requested()) {
      return true;
    }

    for (auto i = 1; i < window_names.size(); ++i) {
      if (!has_webcams[i]) {
        continue;
      }

      opencv_window window(window_names[i], input_video_devices[i].get(), i,
                           window_template.first_time(), has_webcams[i],
                           video_enabled[i], window_template.use_canny(),
                           window_template.low_threshold(),
                           window_template.high_threshold());
      other_window(window);
      if (window.exit_requested()) {
        return true;
      }
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
