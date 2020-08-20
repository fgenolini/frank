#include "config.h"

#if defined(APPLE) && defined(UNIX)

// Implementation of video/audio device enumeration on Apple Mac OSX

WARNINGS_OFF
#include <array>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

#include <gsl/gsl_util>
WARNINGS_ON

using namespace gsl;

#include "device/macos_list_devices.h"
#include "test/testable_cstdio.h"

namespace frank::video {

std::vector<std::string> macos_list_device_names(standard_io *stdio) {
  standard_io default_io{};
  auto io = stdio ? stdio : &default_io;
  constexpr auto SWIFT_SCRIPT =
      "echo "
      "'import AVFoundation;"
      "let devices = AVCaptureDevice.devices(for: .video);"
      "for device in devices {"
      "print(device.localizedName);"
      "}'"
      "|swift -";
  auto pipe = io->popen(SWIFT_SCRIPT, "r");
  if (!pipe)
    return std::vector<std::string>();

  auto _ = finally([&] { io->pclose(pipe); });
  constexpr auto BUFFER_SIZE = 4096;
  std::array<char, BUFFER_SIZE> buffer{};
  std::string result{};
  while (!io->feof(pipe)) {
    if (!io->fgets(buffer.data(), BUFFER_SIZE, pipe))
      break;

    result += buffer.data();
  }

  if (result.empty())
    return std::vector<std::string>();

  std::vector<std::string> device_list{};
  std::stringstream stream(result);
  std::string line{};
  while (std::getline(stream, line))
    device_list.push_back(line);

  return device_list;
}

std::vector<std::string> macos_list_devices(standard_io *stdio) {
  auto device_names = macos_list_device_names(stdio);
  return device_names;
}

} // namespace frank::video

#endif
