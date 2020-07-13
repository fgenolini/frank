#include <iostream>
#include <vector>

#include "config.h"
#include "input_device.h"
#include "linux_list_devices.h"
#include "list_devices.h"
#include "macos_list_devices.h"
#include "win32_list_devices.h"

namespace frank::video {

std::vector<input_device> list_input_devices() {
#if defined(WIN32)
  return win32_list_devices();
#elif defined(UNIX)
#if defined(APPLE)
  return macos_list_devices();
#elif !defined(MINGW) && !defined(MSYS) && !defined(CYGWIN)
  return linux_list_devices();
#else
  std::cout << "Unsupported UNIX / POSIX variant\n";
  std::vector<input_device> no_device{};
  return no_device;
#endif
#else
  std::cout << "Unsupported platform\n";
  std::vector<input_device> no_device{};
  return no_device;
#endif
}

} // namespace frank::video
