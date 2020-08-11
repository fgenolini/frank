#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "input_device.h"

namespace frank::video {

extern std::vector<input_device> list_devices(void *mock_data = nullptr);

}
