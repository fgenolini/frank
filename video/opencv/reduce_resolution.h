#pragma once

#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>
WARNINGS_ON

namespace frank::video {

extern void reduce_resolution(cv::VideoCapture *webcam, int webcam_index);

} // namespace frank::video
