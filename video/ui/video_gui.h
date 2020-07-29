#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "protected_cvui.h"

#include "device/input_device.h"
#include "json/persisted_settings.h"
#include "opencv/opencv_window.h"

namespace frank::video {

class video_gui {
public:
  video_gui(int webcam_count);
  ~video_gui();

  bool loop(std::vector<input_device> &connected_webcams);

private:
  void load_settings();
  void save_settings();
  bool settings_changed() const;

  cv::Mat *overlay_buffers;
  EnhancedWindow settings;
  opencv_window *window_template;
  persisted_settings serialiser_{};
  std::vector<cv::String> overlay_images{};
  std::vector<cv::String> overlay_images_last_{};
  std::vector<cv::String> window_names{};
  std::vector<double> overlay_alpha_last_{};
  std::vector<std::pair<double, double>> height_width_pairs{};
  std::vector<std::unique_ptr<cv::VideoCapture>> input_video_devices{};
  std::vector<bool> has_webcams{};
  double *overlay_alpha;
  bool *overlay_enabled;
  bool *video_enabled;
};

} // namespace frank::video
