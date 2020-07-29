#pragma once

#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>
WARNINGS_ON

#include "opencv/opencv_window.h"

namespace frank::video {

cv::Mat paint_picture(bool first_time, bool has_webcam, bool video_enabled,
                      opencv_window &window, bool use_canny, int low_threshold,
                      int high_threshold, bool overlay_enabled,
                      double overlay_alpha, cv::String const &overlay_image,
                      cv::Mat *overlay_buffer, cv::Mat *raw_picture = nullptr);

} // namespace frank::video
