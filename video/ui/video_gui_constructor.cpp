#include "config.h"

WARNINGS_OFF
#include <cassert>
WARNINGS_ON

#define CVUI_IMPLEMENTATION

#include "opencv/paint_histogram.h"
#include "statistics_window.h"
#include "video_gui.h"

namespace frank::video {

std::unique_ptr<user_interface>
make_user_interface(int webcam_count, cvui_init const &initialise_windows,
                    void *mock_data) {
  return std::make_unique<video_gui>(webcam_count, initialise_windows,
                                     mock_data);
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

video_gui::video_gui(int webcam_count, cvui_init const &initialise_windows,
                     void *mock_data)
    : settings(SETTINGS_X, SETTINGS_Y, SETTINGS_WIDTH, SETTINGS_HEIGHT,
               SETTINGS_TITLE),
      mock_data_(mock_data) {
  window_names.push_back(WINDOW_NAME);
  window_names.push_back(WINDOW1_NAME);
  window_names.push_back(WINDOW2_NAME);
  window_names.push_back(WINDOW3_NAME);
  assert(window_names.size() == MAXIMUM_VIDEO_COUNT);
  for (auto webcam = 0; webcam < (int)window_names.size(); ++webcam) {
    auto has_webcam = webcam < webcam_count;
    has_webcams.push_back(has_webcam);
    histogram_threshold[webcam] = DEFAULT_HISTOGRAM_THRESHOLD;
    overlay_buffers[webcam].addref();
    video_enabled[webcam] = has_webcam;
    if (webcam == 0)
      statistics_.push_back(
          EnhancedWindow(STATISTICS0_X, STATISTICS0_Y, STATISTICS0_WIDTH,
                         STATISTICS0_HEIGHT, STATISTICS_TITLE));
    else
      statistics_.push_back(EnhancedWindow(STATISTICS_X, STATISTICS_Y,
                                           STATISTICS_WIDTH, STATISTICS_HEIGHT,
                                           STATISTICS_TITLE));
  }

  if (!has_webcams[0])
    // Index 0 for OpenCV is also any default video input device, assume there
    // is at least one
    has_webcams[0] = true;

  for ([[maybe_unused]] auto _ : has_webcams) {
    height_width_pairs.push_back(std::make_pair(0.0, 0.0));
    histograms.push_back(false);
    auto input_video_device = std::make_unique<cv::VideoCapture>();
    input_video_devices.push_back(std::move(input_video_device));
    overlay_images.push_back("");
  }

  constexpr auto FIRST_TIME = true;
  constexpr auto HIGH_THRESHOLD = 150;
  constexpr auto LOW_THRESHOLD = 50;
  constexpr auto NO_OVERLAY_ALPHA = 0.0;
  constexpr auto USE_CANNY = false;
  constexpr auto USE_OVERLAY = false;
  constexpr auto WEBCAM_INDEX = 0;
  cv::String overlay_image{};
  window_template = new opencv_window(
      window_names[0], input_video_devices[0].get(), WEBCAM_INDEX, FIRST_TIME,
      has_webcams[0], video_enabled[0], std::make_pair(0.0, 0.0),
      &overlay_buffers[0], USE_CANNY, USE_OVERLAY, overlay_image,
      NO_OVERLAY_ALPHA, LOW_THRESHOLD, HIGH_THRESHOLD, histograms[0]);
  initialise_windows.execute(&window_names[0], window_names.size(), mock_data_);
}

} // namespace frank::video
