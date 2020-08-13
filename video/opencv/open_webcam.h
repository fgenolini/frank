#pragma once

#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>
WARNINGS_ON

#include "opencv_window.h"

namespace frank::video {

extern bool open_webcam(opencv_window &window, cv::VideoCapture *webcam,
                        int webcam_index);

} // namespace frank::video
