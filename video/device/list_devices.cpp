#include "config.h"

WARNINGS_OFF
#include <iostream>
#include <string>
#include <utility>
#include <vector>
WARNINGS_ON

#include "config.h"
#include "input_device.h"
#include "linux_list_devices.h"
#include "list_devices.h"
#include "macos_list_devices.h"
#include "win32_list_devices.h"

namespace frank::video {

std::vector<input_device> list_devices(void *mock_data) {

  auto make_devices = [](std::vector<std::string> device_names) {
    std::vector<input_device> returned_devices{};
    for (auto const &device_name : std::as_const(device_names)) {
      input_device returned_device(device_name);
      returned_devices.push_back(returned_device);
    }

    return returned_devices;
  };

  auto get_device_names = [&]() {
#if defined(WIN32)
    return win32_list_devices(mock_data);
#elif defined(UNIX)
#if defined(APPLE)
    return macos_list_devices(mock_data);
#elif !defined(MINGW) && !defined(MSYS) && !defined(CYGWIN)
    return linux_list_devices(mock_data);
#else
    std::cout << "Unsupported UNIX / POSIX variant\n";
    std::vector<std::string> no_device{};
    return no_device;
#endif
#else
    std::cout << "Unsupported platform\n";
    std::vector<std::string> no_device{};
    return no_device;
#endif
  };

  return make_devices(get_device_names());
}

} // namespace frank::video
