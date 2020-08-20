#include "config.h"

WARNINGS_OFF
#include <cassert>
WARNINGS_ON

#define CVUI_IMPLEMENTATION

#include "opencv/paint_histogram.h"
#include "ui/statistics_window.h"
#include "ui/video_gui.h"

namespace frank::video {

user_interface_factory::~user_interface_factory() {}

std::unique_ptr<user_interface>
user_interface_factory::make(int webcam_count, cvui_init *initialise_windows,
                             ui_controls &controls, file_dialogs &dialogs) {
  return std::make_unique<video_gui>(webcam_count, initialise_windows, controls,
                                     dialogs);
}

constexpr auto SETTINGS_HEIGHT = 280;
constexpr auto SETTINGS_TITLE = "Settings";
constexpr auto SETTINGS_WIDTH = 600;
constexpr auto SETTINGS_X = 10;
constexpr auto SETTINGS_Y = 50;
constexpr auto STATISTICS0_HEIGHT = 400;
constexpr auto STATISTICS0_WIDTH = 500;
constexpr auto STATISTICS0_X = 110;
constexpr auto STATISTICS0_Y = 10;
constexpr auto STATISTICS_HEIGHT = 150;
constexpr auto STATISTICS_WIDTH = 200;
constexpr auto STATISTICS_X = 50;
constexpr auto STATISTICS_Y = 10;
constexpr auto WINDOW_NAME = "Frank video";
constexpr auto WINDOW1_NAME = "Frank video 1";
constexpr auto WINDOW2_NAME = "Frank video 2";
constexpr auto WINDOW3_NAME = "Frank video 3";

video_gui::video_gui(int webcam_count, cvui_init *initialise_windows,
                     ui_controls &controls, file_dialogs &dialogs)
    : settings_(SETTINGS_X, SETTINGS_Y, SETTINGS_WIDTH, SETTINGS_HEIGHT,
                SETTINGS_TITLE),
      dialogs_(dialogs), controls_(controls) {
  window_names_.push_back(WINDOW_NAME);
  window_names_.push_back(WINDOW1_NAME);
  window_names_.push_back(WINDOW2_NAME);
  window_names_.push_back(WINDOW3_NAME);
  auto const expected_window_count = state_.has_webcams.size();
  assert(window_names_.size() == expected_window_count);
  for (auto webcam = 0; webcam < (int)expected_window_count; ++webcam) {
    auto has_webcam = webcam < webcam_count;
    state_.has_webcams[(size_t)webcam] = has_webcam;
    histogram_threshold_.push_back(DEFAULT_HISTOGRAM_THRESHOLD);
    overlay_buffers_.push_back(cv::Mat());
    state_.video_enabled[(size_t)webcam] = has_webcam;
    if (webcam == 0)
      statistics_.push_back(
          EnhancedWindow(STATISTICS0_X, STATISTICS0_Y, STATISTICS0_WIDTH,
                         STATISTICS0_HEIGHT, STATISTICS_TITLE));
    else
      statistics_.push_back(EnhancedWindow(STATISTICS_X, STATISTICS_Y,
                                           STATISTICS_WIDTH, STATISTICS_HEIGHT,
                                           STATISTICS_TITLE));
  }

  // Index 0 for OpenCV is also any default video input device, assume there
  // is at least one
  if (!state_.has_webcams[0])
    state_.has_webcams[0] = true;

  for ([[maybe_unused]] auto _ : state_.has_webcams) {
    height_width_pairs_.push_back(std::make_pair(0.0, 0.0));
    histograms_.push_back(false);
    auto input_video_device = std::make_unique<cv::VideoCapture>();
    input_video_devices_.push_back(std::move(input_video_device));
  }

  constexpr auto FIRST_TIME = true;
  constexpr auto HIGH_THRESHOLD = 150;
  constexpr auto LOW_THRESHOLD = 50;
  constexpr auto NO_OVERLAY_ALPHA = 0.0;
  constexpr auto USE_OVERLAY = false;
  constexpr auto WEBCAM_INDEX = 0;
  cv::String overlay_image{};
  window_template_ = opencv_window(
      window_names_[0], input_video_devices_[0].get(), WEBCAM_INDEX, FIRST_TIME,
      state_.has_webcams[0], state_.video_enabled[0], std::make_pair(0.0, 0.0),
      &overlay_buffers_[0], USE_OVERLAY, overlay_image, NO_OVERLAY_ALPHA,
      LOW_THRESHOLD, HIGH_THRESHOLD, histograms_[0]);
  if (initialise_windows)
    initialise_windows->execute(&window_names_[0], window_names_.size());
}

video_gui::~video_gui() {
  for (auto &video_capture : input_video_devices_) {
    auto capturing_device = video_capture.get();
    capturing_device->release();
  }
}

} // namespace frank::video
