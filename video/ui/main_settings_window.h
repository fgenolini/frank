#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "protected_cvui.h"

#include "device/input_device.h"
#include "ui/ui_controls.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4820)
class main_settings_window {
public:
  main_settings_window(ui_controls const &controls, bool *video_enabled_array,
                       bool *use_canny, int *low_threshold, int *high_threshold,
                       bool *overlay_enabled_array, double *overlay_alpha_array,
                       void *mock_data = nullptr);

  void draw(bool settings_minimised,
            std::vector<input_device> const &input_devices,
            std::vector<bool> &has_webcams,
            std::vector<cv::String> &overlay_images);

private:
  void draw_canny();
  void draw_webcam(int webcam, std::vector<input_device> const &input_devices);
  void draw_overlay(int webcam, std::vector<cv::String> &overlay_images);

  ui_controls controls_;
  double *overlay_alpha_array_;
  int *high_threshold_;
  int *low_threshold_;
  bool *overlay_enabled_array_;
  bool *use_canny_;
  bool *video_enabled_array_;
  void *mock_data_;
};
WARNINGS_ON

} // namespace frank::video
