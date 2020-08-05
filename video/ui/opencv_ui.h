#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"

namespace frank::video {

#if !defined(_TEST_RUN_APPLICATION_2_)
[[noreturn]]
#endif
extern void
opencv_ui(std::vector<input_device> &connected_webcams);

} // namespace frank::video
