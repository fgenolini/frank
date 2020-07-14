#pragma once

#include <string>
#include <vector>

namespace frank::video {

extern std::vector<std::string> win32_list_devices(
    std::vector<std::string> const *mocked_device_names = nullptr);

}
