#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "test/testable_cstdio.h"

namespace frank::video {

class video_devices {
public:
  video_devices(standard_io *stdio = nullptr);
  virtual ~video_devices();

  virtual std::vector<input_device> list();

private:
  standard_io *stdio_;
};

} // namespace frank::video
