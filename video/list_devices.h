#pragma once

#include <string>
#include <vector>

#include "input_device.h"

namespace frank::video {

extern std::vector<input_device> list_input_devices(
    std::vector<std::string> const *mocked_device_names = nullptr);

}
