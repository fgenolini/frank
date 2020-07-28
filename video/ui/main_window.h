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
#include "opencv/opencv_window.h"

namespace frank::video {

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled,
                 bool *overlay_enabled, double *overlay_alpha,
                 std::vector<cv::String> &overlay_images,
                 opencv_window &window);

} // namespace frank::video
