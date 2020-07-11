#include <array>
#include <cstdlib>
#include <memory>

#include <gsl/gsl_util>

using namespace gsl;

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

// EnhancedWindow.h must be included after cvui.h
#include "EnhancedWindow.h"

#include "input_device.h"
#include "opencv_ui.h"
#include "opencv_window.h"

namespace frank::video {

bool opencv_with_webcams(std::vector<input_device> &connected_webcams) {
  constexpr auto MAXIMUM_VIDEO_COUNT = 4;
  constexpr auto WINDOW_NAME = "Frank video";
  constexpr auto WINDOW1_NAME = "Frank video 1";
  constexpr auto WINDOW2_NAME = "Frank video 2";
  constexpr auto WINDOW3_NAME = "Frank video 3";

  auto exit_requested = []() {
    constexpr auto ESCAPE_KEY = 27;
    constexpr auto SHORT_DELAY = 20;

    return cv::waitKey(SHORT_DELAY) == ESCAPE_KEY;
  };

  auto take_picture = [exit_requested](cv::VideoCapture *webcam,
                                       int webcam_index) {
    cv::Mat pic{};
    while (!webcam->isOpened()) {
      std::cout << "Could not open webcam " << webcam_index << '\n';
      webcam->open(webcam_index, cv::CAP_ANY);
      if (exit_requested()) {
        return pic;
      }
    }

    *webcam >> pic;
    return pic;
  };

  auto main_window = [take_picture](EnhancedWindow &settings,
                                    std::vector<input_device> &input_devices,
                                    std::vector<bool> &has_webcams,
                                    bool *video_enabled,
                                    opencv_window &window) {
    auto use_canny = window.use_canny();
    auto low_threshold = window.low_threshold();
    auto high_threshold = window.high_threshold();
    auto main_frame = cv::Mat(200, 500, CV_8UC3);
    main_frame = cv::Scalar(49, 52, 49);
    cvui::context(window.name());
    if (has_webcams[0] && video_enabled[0]) {
      auto picture = take_picture(window.webcam(), 0);
      if (use_canny) {
        cv::cvtColor(picture, main_frame, cv::COLOR_BGR2GRAY);
        cv::Canny(main_frame, main_frame, low_threshold, high_threshold, 3);
        cv::cvtColor(main_frame, main_frame, cv::COLOR_GRAY2BGR);
      } else {
        main_frame = picture;
      }
    }

    auto button_clicked = cvui::button(main_frame, 110, 80, "Quit");
    if (button_clicked) {
      return true;
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
    return false;
  };

  auto other_window = [take_picture](opencv_window const &window) {
    auto use_canny = window.use_canny();
    auto low_threshold = window.low_threshold();
    auto high_threshold = window.high_threshold();
    auto other_frame = cv::Mat(200, 500, CV_8UC3);
    other_frame = cv::Scalar(49, 52, 49);
    cvui::context(window.name());
    if (window.has_webcam() && window.video_enabled()) {
      auto picture = take_picture(window.webcam(), window.webcam_index());
      if (use_canny) {
        cv::cvtColor(picture, other_frame, cv::COLOR_BGR2GRAY);
        cv::Canny(other_frame, other_frame, low_threshold, high_threshold, 3);
        cv::cvtColor(other_frame, other_frame, cv::COLOR_GRAY2BGR);
      } else {
        other_frame = picture;
      }
    }

    cvui::imshow(window.name(), other_frame);
  };

  std::vector<cv::String> window_names = {WINDOW_NAME, WINDOW1_NAME,
                                          WINDOW2_NAME, WINDOW3_NAME};
  bool video_enabled[MAXIMUM_VIDEO_COUNT]{};
  std::vector<bool> has_webcams{};
  for (auto i = 0; i < window_names.size() && i < MAXIMUM_VIDEO_COUNT; ++i) {
    auto has_webcam = connected_webcams.size() > i;
    has_webcams.push_back(has_webcam);
    video_enabled[i] = has_webcam;
  }

  std::vector<std::unique_ptr<cv::VideoCapture>> input_video_devices{};
  for (auto i = 0; i < has_webcams.size(); ++i) {
    if (!has_webcams[i]) {
      input_video_devices.push_back(
          std::move(std::unique_ptr<cv::VideoCapture>()));
      continue;
    }

    auto webcam = std::make_unique<cv::VideoCapture>(i);
    input_video_devices.push_back(std::move(webcam));
  }

  auto _ = finally([&input_video_devices] {
    std::cout << "Releasing webcams" << '\n';
    for (auto &video_capture : input_video_devices) {
      auto capturing_device = video_capture.get();
      capturing_device->release();
    }
  });

  opencv_window window_template(window_names[0], input_video_devices[0].get(),
                                0, has_webcams[0], video_enabled[0], false, 50,
                                150);
  EnhancedWindow settings(200, 50, 250, 250, "Settings");
  cvui::init(&window_names[0], window_names.size());
  while (true) {
    if (main_window(settings, connected_webcams, has_webcams, video_enabled,
                    window_template)) {
      return true;
    }

    for (auto i = 1; i < window_names.size(); ++i) {
      if (!has_webcams[i]) {
        continue;
      }

      opencv_window window(
          window_names[i], input_video_devices[i].get(), i, has_webcams[i],
          video_enabled[i], window_template.use_canny(),
          window_template.low_threshold(), window_template.high_threshold());
      other_window(window);
    }

    if (exit_requested()) {
      return true;
    }
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
