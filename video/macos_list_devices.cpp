#include "config.h"

#if defined(APPLE) && defined(UNIX) && !defined(WIN32)

// Implementation of video/audio device enumeration on Apple Mac OSX

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

#include <gsl/gsl_util>

using namespace gsl;

#include "input_device.h"
#include "macos_list_devices.h"

namespace frank::video {

std::vector<input_device> macos_list_devices() {
  std::cout << "AVFoundation input devices\n";
  std::string swift_script{"echo "
                           "'import AVFoundation;"
                           "print(\"-----\");"
                           "let devices = AVCaptureDevice.devices(for: .video);"
                           "for device in devices {"
                           "print(\"---\");"
                           "print(device.localizedName);"
                           "print(device.modelID);"
                           "print(device.activeFormat);"
                           "print(\"-\");"
                           "print(device.formats)"
                           "}'"
                           "|swift -"};
  system(swift_script.c_str());

  // TODO: read stdout from swift script
  std::vector<input_device> no_device{};
  return no_device;
}

} // namespace frank::video

#endif
