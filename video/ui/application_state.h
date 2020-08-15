#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

#include "device/input_device.h"

namespace frank::video {

constexpr auto MAXIMUM_VIDEO_COUNT = 4;

class application_state {
public:
  application_state(int webcam_count);

  std::vector<bool> has_webcams{};
  std::vector<bool> overlay_enabled{};
  std::vector<bool> video_enabled{};
  std::vector<double> overlay_alpha{};
  std::vector<input_device> input_devices{};
  std::vector<std::string> overlay_images{};
  int high_threshold{};
  int low_threshold{};
  bool use_canny{};
};

} // namespace frank::video
