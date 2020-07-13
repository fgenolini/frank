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

#include "input_device.h"
#include "macos_list_devices.h"

namespace frank::video {

std::vector<input_device> macos_list_devices() {
  std::cout << "AVFoundation input devices\n";
  std::string swift_script{"echo "
                           "'import AVFoundation;"
                           "let devices = AVCaptureDevice.devices(for: .video);"
                           "for device in devices {"
                           "print(device.localizedName);"
                           "}'"
                           "|swift -"};
  auto pipe = popen(swift_script.c_str(), "r");
  if (!pipe) {
    std::vector<input_device> no_device{};
    return no_device;
  }

  auto _ = finally([pipe] { pclose(pipe); });
  constexpr auto BUFFER_SIZE = 4096;
  std::array<char, BUFFER_SIZE> buffer{};
  std::string result{};
  while (!feof(pipe)) {
    if (!fgets(buffer.data(), BUFFER_SIZE, pipe)) {
      break;
    }

    result += buffer.data();
  }

  std::vector<input_device> device_list{};
  std::stringstream stream(result);
  std::string line{};
  while (std::getline(stream, line)) {
    input_device new_device(line);
    device_list.push_back(new_device);
  }

  return device_list;
}

} // namespace frank::video

#endif
