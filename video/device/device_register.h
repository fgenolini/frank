#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

namespace frank::video {

// Abstract interface for getting a list of video input devices
// currently connected to the local computer
class device_register {
public:
  virtual ~device_register() = default;

  virtual std::vector<std::string> const &name_devices() const = 0;
};

} // namespace frank::video
