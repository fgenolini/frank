#pragma once

#include "protected_cvui.h"

namespace frank::video {

constexpr auto STATISTICS_TITLE = "RGB hist";

void statistics_window(EnhancedWindow &statistics, cv::Mat &frame,
                       cv::Mat const &raw_picture, int *tone_down_threshold);

} // namespace frank::video
