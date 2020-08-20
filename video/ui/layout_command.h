#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>

#include <opencv2/opencv.hpp>
WARNINGS_ON

namespace frank::video {

struct layout_command {
  void begin_column();
  void begin_row();
  void begin_row(cv::Mat &frame, int x, int y);
  void context(std::string const &window_name);
  void end_column();
  void end_row();
  void imshow(std::string const &window_name, cv::Mat image);
};

} // namespace frank::video
