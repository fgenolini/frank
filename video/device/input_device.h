#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

class input_device {
public:
  input_device();
  input_device(std::string name);

  bool operator==(input_device const &other) const;

  std::string name() const;

private:
  /// <summary>
  /// Localised name for the video input device / webcam
  /// </summary>
  std::string name_;
};

} // namespace frank::video
