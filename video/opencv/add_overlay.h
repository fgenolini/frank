#pragma once

#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif

#include <opencv2/opencv.hpp>

#if defined(WIN32)
#pragma warning(pop)
#endif

namespace frank::video {

cv::Mat add_overlay(double overlay_alpha, cv::String const &overlay_image,
                    cv::Mat *overlay_buffer, cv::Mat const &frame);

} // namespace frank::video
