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

#include "opencv_window.h"

namespace frank::video {

cv::Mat take_picture(opencv_window &window) noexcept(false);

} // namespace frank::video
