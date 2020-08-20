#pragma once

#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>
WARNINGS_ON

namespace frank::video {

extern cv::Mat add_overlay(double overlay_alpha,
                           cv::String const &overlay_image,
                           cv::Mat *overlay_buffer, cv::Mat const &frame);

} // namespace frank::video
