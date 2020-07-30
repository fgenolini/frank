#include "config.h"

#include "protected_cvui.h"

#include "opencv/paint_histogram.h"
#include "statistics_window.h"

namespace frank::video {

void statistics_window(EnhancedWindow &statistics, cv::Mat &frame,
                       cv::Mat const &raw_picture, int *tone_down_threshold) {
  if (frame.empty() || statistics.isMinimized()) {
    return;
  }

  statistics.setHeight(frame.rows - statistics.posY() - 2);
  statistics.setWidth(frame.cols - statistics.posX() - 2);
  statistics.begin(frame);
  cvui::beginColumn();
  {
    auto threshold = DEFAULT_HISTOGRAM_THRESHOLD;
    if (tone_down_threshold && *tone_down_threshold > 0 &&
        *tone_down_threshold <= MAXIMUM_THRESHOLD) {
      threshold = *tone_down_threshold;
    }

    cvui::trackbar<int>(statistics.width() - 20, &threshold, 1,
                        MAXIMUM_THRESHOLD);
    auto histogram_image =
        paint_histogram(statistics.height() - 80, statistics.width() - 20,
                        raw_picture, threshold);
    if (tone_down_threshold) {
      *tone_down_threshold = threshold;
    }

    try {
      cvui::image(histogram_image);
    } catch (...) {
    }
  }
  cvui::endColumn();
  statistics.end();
}

} // namespace frank::video
