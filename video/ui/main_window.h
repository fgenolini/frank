#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "protected_cvui.h"

#include "device/input_device.h"
#include "opencv/opencv_window.h"

namespace frank::video {

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled,
                 bool *overlay_enabled, double *overlay_alpha,
                 std::vector<cv::String> &overlay_images,
                 opencv_window &window);

} // namespace frank::video
