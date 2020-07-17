#pragma once

#include <string>
#include <vector>

#include "device_register.h"

namespace frank::video {

extern std::vector<std::string>
win32_list_devices(device_register const *name_devices = nullptr);

}
