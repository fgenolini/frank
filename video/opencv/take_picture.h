#pragma once

#include <opencv2/opencv.hpp>

#include "opencv_window.h"

namespace frank::video {

cv::Mat take_picture(opencv_window &window) noexcept(false);

} // namespace frank::video
