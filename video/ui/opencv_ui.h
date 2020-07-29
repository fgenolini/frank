#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"

namespace frank::video {

[[noreturn]] extern void
opencv_ui(std::vector<input_device> &connected_webcams);

} // namespace frank::video
