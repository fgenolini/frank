#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

namespace frank::video {

extern std::vector<std::string> macos_list_devices(void *mock_data = nullptr);

}
