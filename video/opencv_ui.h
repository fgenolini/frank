#pragma once

#include <vector>

#include "input_device.h"

namespace frank::video {

[[noreturn]] extern void
opencv_ui(std::vector<input_device> &connected_webcams);

} // namespace frank::video
