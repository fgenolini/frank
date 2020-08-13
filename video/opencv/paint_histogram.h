#pragma once

#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>
WARNINGS_ON

namespace frank::video {

constexpr auto DEFAULT_HISTOGRAM_THRESHOLD = 32;
constexpr auto MAXIMUM_THRESHOLD = 400;

extern cv::Mat paint_histogram(int height, int width,
                               cv::Mat const &raw_picture,
                               int tone_down_threshold);

} // namespace frank::video
