#pragma once

#include <memory>

#define CVUI_DISABLE_COMPILATION_NOTICES
#include "cvui.h"

// EnhancedWindow.h must be included after cvui.h
#include "EnhancedWindow.h"

#include "../device/input_device.h"
#include "../opencv/opencv_window.h"

namespace frank::video {

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled,
                 opencv_window &window);

} // namespace frank::video
