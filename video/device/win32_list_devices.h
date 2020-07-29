#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

#include "device_register.h"

namespace frank::video {

extern std::vector<std::string>
win32_list_devices(device_register const *name_devices = nullptr);

}
