#pragma once

#include "config.h"

#include <string>
#include <vector>

#include "device_register.h"

namespace frank::video {

extern std::vector<std::string>
linux_list_devices(device_register const *name_devices = nullptr);

}
