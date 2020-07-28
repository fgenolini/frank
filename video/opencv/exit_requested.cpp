#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif
#include <opencv2/opencv.hpp>

#if defined(WIN32)
#pragma warning(pop)
#endif

#include "exit_requested.h"

namespace frank::video {

bool exit_requested() {
  constexpr auto ESCAPE_KEY = 27;
  constexpr auto SHORT_DELAY = 20;

  return cv::waitKey(SHORT_DELAY) == ESCAPE_KEY;
}

} // namespace frank::video
