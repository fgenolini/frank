#pragma once

#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif

#include <vector>

#if defined(WIN32)
#pragma warning(pop)
#endif

#include "protected_cvui.h"

#include "device/input_device.h"
#include "json/persisted_settings.h"
#include "opencv/opencv_window.h"

namespace frank::video {

constexpr auto ALPHA = "alpha_";
constexpr auto MAXIMUM_VIDEO_COUNT = 4;
constexpr auto OVERLAY = "overlay_";
constexpr auto SETTINGS_HEIGHT = 280;
constexpr auto SETTINGS_TITLE = "Settings";
constexpr auto SETTINGS_WIDTH = 600;
constexpr auto SETTINGS_X = 10;
constexpr auto SETTINGS_Y = 50;
constexpr auto WINDOW_NAME = "Frank video";
constexpr auto WINDOW1_NAME = "Frank video 1";
constexpr auto WINDOW2_NAME = "Frank video 2";
constexpr auto WINDOW3_NAME = "Frank video 3";

class video_gui {
public:
  video_gui(int webcam_count);
  ~video_gui();

  bool loop(std::vector<input_device> &connected_webcams);

private:
  void load_settings();
  void save_settings();
  bool settings_changed() const;

  cv::Mat overlay_buffers[MAXIMUM_VIDEO_COUNT]{};
  EnhancedWindow settings{SETTINGS_X, SETTINGS_Y, SETTINGS_WIDTH,
                          SETTINGS_HEIGHT, SETTINGS_TITLE};
  opencv_window *window_template;
  persisted_settings serialiser_{};
  std::vector<cv::String> overlay_images{};
  std::vector<cv::String> overlay_images_last_{};
  std::vector<cv::String> window_names{WINDOW_NAME, WINDOW1_NAME, WINDOW2_NAME,
                                       WINDOW3_NAME};
  std::vector<double> overlay_alpha_last_{};
  std::vector<std::pair<double, double>> height_width_pairs{};
  std::vector<std::unique_ptr<cv::VideoCapture>> input_video_devices{};
  std::vector<bool> has_webcams{};
  double overlay_alpha[MAXIMUM_VIDEO_COUNT]{};
  bool overlay_enabled[MAXIMUM_VIDEO_COUNT]{};
  bool video_enabled[MAXIMUM_VIDEO_COUNT]{};
};

} // namespace frank::video
