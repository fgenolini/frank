#pragma once

#include <opencv2/opencv.hpp>

namespace frank::video {

cv::Mat add_overlay(double overlay_alpha, cv::String const &overlay_image,
                    cv::Mat *overlay_buffer, cv::Mat const &frame);

} // namespace frank::video
