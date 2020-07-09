#include <array>
#include <cstdlib>
#include <memory>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

// EnhancedWindow.h must be included after cvui.h
#include "EnhancedWindow.h"

#include "input_device.h"
#include "opencv_ui.h"

namespace frank::video {

[[noreturn]] void opencv_ui(std::vector<input_device> &connected_webcams) {
  constexpr auto MAXIMUM_VIDEO_COUNT = 4;
  constexpr auto WINDOW_NAME = "Frank video";
  constexpr auto WINDOW1_NAME = "Frank video 1";
  constexpr auto WINDOW2_NAME = "Frank video 2";
  constexpr auto WINDOW3_NAME = "Frank video 3";

  auto wait_or_exit = []() {
    constexpr auto ESCAPE_KEY = 27;
    constexpr auto SHORT_DELAY = 20;

    if (cv::waitKey(SHORT_DELAY) == ESCAPE_KEY) {
      exit(EXIT_SUCCESS);
    }
  };

  auto take_picture = [wait_or_exit](cv::VideoCapture *webcam,
                                     int webcam_index) {
    while (!webcam->isOpened()) {
      std::cout << "Could not open webcam " << webcam_index << '\n';
      webcam->open(webcam_index, cv::CAP_ANY);
      wait_or_exit();
    }

    cv::Mat pic;
    *webcam >> pic;
    return pic;
  };

  auto main_window =
      [take_picture](const cv::String &window_name, EnhancedWindow &settings,
                     std::vector<input_device> &input_devices,
                     std::vector<bool> &has_webcams, cv::VideoCapture *webcam,
                     bool *video_enabled) {
        auto main_frame = cv::Mat(200, 500, CV_8UC3);
        main_frame = cv::Scalar(49, 52, 49);
        cvui::context(window_name);
        if (has_webcams[0] && video_enabled[0]) {
          main_frame = take_picture(webcam, 0);
        }

        auto button_clicked = cvui::button(main_frame, 110, 80, "Quit");
        if (button_clicked) {
          exit(EXIT_SUCCESS);
        }

        settings.begin(main_frame);
        if (!settings.isMinimized()) {
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
        cvui::imshow(window_name, main_frame);
      };

  auto other_window = [take_picture](const cv::String &window_name,
                                     bool has_webcam, bool video_enabled,
                                     int webcam_index,
                                     cv::VideoCapture *webcam) {
    auto other_frame = cv::Mat(200, 500, CV_8UC3);
    other_frame = cv::Scalar(49, 52, 49);
    cvui::context(window_name);
    if (has_webcam && video_enabled) {
      other_frame = take_picture(webcam, webcam_index);
    }

    cvui::imshow(window_name, other_frame);
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

  EnhancedWindow settings(200, 50, 250, 100, "Settings");
  cvui::init(&window_names[0], window_names.size());
  while (true) {
    main_window(window_names[0], settings, connected_webcams, has_webcams,
                input_video_devices[0].get(), video_enabled);
    for (auto i = 1; i < window_names.size(); ++i) {
      if (!has_webcams[i]) {
        continue;
      }

      other_window(window_names[i], has_webcams[i], video_enabled[i], i,
                   input_video_devices[i].get());
    }

    wait_or_exit();
  }

  exit(EXIT_SUCCESS);
}

} // namespace frank::video
