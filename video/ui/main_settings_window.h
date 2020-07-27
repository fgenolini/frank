#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

#define CVUI_DISABLE_COMPILATION_NOTICES
#include "cvui.h"

// EnhancedWindow.h must be included after cvui.h
#include "EnhancedWindow.h"

#include "../device/input_device.h"

namespace frank::video {

class main_settings_window {
public:
  main_settings_window(bool *video_enabled_array, bool *use_canny,
                       int *low_threshold, int *high_threshold,
                       bool *overlay_enabled_array, double *overlay_alpha_array)
      : overlay_alpha_array_(overlay_alpha_array),
        high_threshold_(high_threshold), low_threshold_(low_threshold),
        overlay_enabled_array_(overlay_enabled_array), use_canny_(use_canny),
        video_enabled_array_(video_enabled_array) {}

  void draw(EnhancedWindow &settings, std::vector<input_device> &input_devices,
            std::vector<bool> &has_webcams,
            std::vector<cv::String> &overlay_images);

private:
  void draw_canny();
  void draw_webcam(int webcam, std::vector<input_device> &input_devices);
  void draw_overlay(int webcam, std::vector<cv::String> &overlay_images);

  double *overlay_alpha_array_;
  int *high_threshold_;
  int *low_threshold_;
  bool *overlay_enabled_array_;
  bool *use_canny_;
  bool *video_enabled_array_;
};

} // namespace frank::video
