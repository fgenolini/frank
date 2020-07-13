#pragma once

#include <vector>

#include "input_device.h"

namespace frank::video {

extern std::vector<input_device> linux_list_devices();

}
