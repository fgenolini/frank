#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>
WARNINGS_ON

#include "exit_requested.h"

namespace frank::video {

bool exit_requested() {
  constexpr auto ESCAPE_KEY = 27;
  constexpr auto SHORT_DELAY = 20;

  return cv::waitKey(SHORT_DELAY) == ESCAPE_KEY;
}

} // namespace frank::video
