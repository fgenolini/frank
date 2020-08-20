#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

#include "test/testable_cstdio.h"

namespace frank::video {

extern std::vector<std::string>
win32_list_devices(standard_io *stdio = nullptr);

}
