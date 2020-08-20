#include "config.h"

WARNINGS_OFF
#include <iostream>
#include <string>
#include <utility>
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "device/linux_list_devices.h"
#include "device/macos_list_devices.h"
#include "device/video_devices.h"
#include "device/win32_list_devices.h"

namespace frank::video {

video_devices::video_devices(standard_io *stdio) : stdio_(stdio) {}

video_devices::~video_devices() {}

std::vector<input_device> video_devices::list() {

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
    return win32_list_devices(stdio_);
#elif defined(UNIX)
#if defined(APPLE)
    return macos_list_devices(stdio_);
#elif !defined(MINGW) && !defined(MSYS) && !defined(CYGWIN)
    return linux_list_devices(stdio_);
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
