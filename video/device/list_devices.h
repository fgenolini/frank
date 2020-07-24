#pragma once

#include <string>
#include <vector>

#include "device_register.h"
#include "input_device.h"

namespace frank::video {

extern std::vector<input_device>
list_input_devices(device_register const *name_devices = nullptr);

}
