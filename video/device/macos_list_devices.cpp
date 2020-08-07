#include "config.h"

#if defined(APPLE) && defined(UNIX) && !defined(WIN32)

// Implementation of video/audio device enumeration on Apple Mac OSX

#include <array>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gsl/gsl_util>

using namespace gsl;

#include "macos_list_devices.h"

namespace frank::video {

std::vector<std::string> macos_list_device_names() {
  std::cout << "AVFoundation input devices\n";
  std::string swift_script{"echo "
                           "'import AVFoundation;"
                           "let devices = AVCaptureDevice.devices(for: .video);"
                           "for device in devices {"
                           "print(device.localizedName);"
                           "}'"
                           "|swift -"};
  auto pipe = popen(swift_script.c_str(), "r");
  if (!pipe)
    return std::vector<std::string>();

  auto _ = finally([pipe] { pclose(pipe); });
  constexpr auto BUFFER_SIZE = 4096;
  std::array<char, BUFFER_SIZE> buffer{};
  std::string result{};
  while (!feof(pipe)) {
    if (!fgets(buffer.data(), BUFFER_SIZE, pipe))
      break;

    result += buffer.data();
  }

  std::vector<std::string> device_list{};
  std::stringstream stream(result);
  std::string line{};
  while (std::getline(stream, line))
    device_list.push_back(line);

  return device_list;
}

std::vector<std::string>
macos_list_devices(device_register const *name_devices) {
  auto device_names = macos_list_device_names();
  if (name_devices)
    device_names = name_devices->name_devices();
  else
    std::cout << device_names.size() << " video input devices\n";

  return device_names;
}

} // namespace frank::video

#endif
