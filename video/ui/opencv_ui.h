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

#include "device/input_device.h"

namespace frank::video {

[[noreturn]] extern void
opencv_ui(std::vector<input_device> &connected_webcams);

} // namespace frank::video
