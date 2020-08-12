#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "protected_cvui.h"

#include "device/input_device.h"
#include "ui/button_command.h"
#include "ui/checkbox_command.h"
#include "ui/layout_command.h"
#include "ui/text_command.h"
#include "ui/trackbar_double_command.h"
#include "ui/trackbar_int_command.h"

namespace frank::video {

class main_settings_window {
public:
  main_settings_window(button_command const &button,
                       checkbox_command const &checkbox,
                       layout_command const &layout, text_command const &text,
                       trackbar_double_command const &trackbar_double,
                       trackbar_int_command const &trackbar_int,
                       bool *video_enabled_array, bool *use_canny,
                       int *low_threshold, int *high_threshold,
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

  button_command button_;
  checkbox_command checkbox_;
  layout_command layout_;
  text_command text_;
  trackbar_double_command trackbar_double_;
  trackbar_int_command trackbar_int_;
  double *overlay_alpha_array_;
  int *high_threshold_;
  int *low_threshold_;
  bool *overlay_enabled_array_;
  bool *use_canny_;
  bool *video_enabled_array_;
  void *mock_data_;
};

} // namespace frank::video
