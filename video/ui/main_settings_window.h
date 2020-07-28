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

namespace frank::video {

class main_settings_window {
public:
  main_settings_window(bool *video_enabled_array, bool *use_canny,
                       int *low_threshold, int *high_threshold,
                       bool *overlay_enabled_array,
                       double *overlay_alpha_array);

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
