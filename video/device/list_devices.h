#pragma once

#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif

#include <string>
#include <vector>

#if defined(WIN32)
#pragma warning(pop)
#endif

#include "device_register.h"
#include "input_device.h"

namespace frank::video {

extern std::vector<input_device>
list_input_devices(device_register const *name_devices = nullptr);

}
