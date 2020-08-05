#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

#include "device_register.h"
#include "input_device.h"

namespace frank::video {

extern std::vector<input_device>
list_input_devices(device_register const *name_devices = nullptr,
                   void *mock_data = nullptr);

}
