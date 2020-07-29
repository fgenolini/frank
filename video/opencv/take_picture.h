#pragma once

#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>
WARNINGS_ON

#include "opencv_window.h"

namespace frank::video {

cv::Mat take_picture(opencv_window &window) noexcept(false);

} // namespace frank::video
