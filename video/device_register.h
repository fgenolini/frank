#pragma once

#include <string>
#include <vector>

namespace frank::video {

// Abstract interface for getting a list of video input devices
// currently connected to the local computer
class device_register {
public:
  virtual ~device_register() = default;

  virtual std::vector<std::string> const &name_devices() const = 0;
};

} // namespace frank::video
