#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "protected_cvui.h"

#include "statistics_window.h"

namespace frank::video {

constexpr auto HISTOGRAM_SIZE = 128;
constexpr auto MAXIMUM_THRESHOLD = 400;

void tone_down(int pixel_count, int threshold, cv::Mat &red, cv::Mat &green,
               cv::Mat &blue) {
  auto const most = pixel_count / threshold;
  auto const reduce_to = most / 5;
  blue.setTo(reduce_to, blue > most);
  green.setTo(reduce_to, green > most);
  red.setTo(reduce_to, red > most);
}

cv::Mat paint_histogram(int height, int width, cv::Mat const &raw_picture,
                        int tone_down_threshold) {
  if (raw_picture.empty() || height < 1 || width < 1) {
    return cv::Mat();
  }

  std::vector<cv::Mat> bgr_planes{};
  cv::split(raw_picture, bgr_planes);
  auto const bin_count = HISTOGRAM_SIZE;
  float range[] = {0.0f, 256.0f}; // the upper boundary is exclusive
  const float *ranges = {range};
  auto const uniform = true;
  auto const accumulate = false;
  cv::Mat blue{};
  cv::Mat green{};
  cv::Mat red{};
  cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), blue, 1, &bin_count, &ranges,
               uniform, accumulate);
  cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), green, 1, &bin_count, &ranges,
               uniform, accumulate);
  cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), red, 1, &bin_count, &ranges,
               uniform, accumulate);
  tone_down(raw_picture.cols * raw_picture.rows, tone_down_threshold, red,
            green, blue);
  auto bin_w = cvRound((double)width / (double)bin_count);
  cv::Mat image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::normalize(blue, blue, 0, image.rows, cv::NORM_MINMAX, -1, cv::Mat());
  cv::normalize(green, green, 0, image.rows, cv::NORM_MINMAX, -1, cv::Mat());
  cv::normalize(red, red, 0, image.rows, cv::NORM_MINMAX, -1, cv::Mat());
  constexpr auto LINE_WIDTH = 1;
  for (auto i = 1; i < bin_count; ++i) {
    cv::line(image,
             cv::Point(bin_w * (i - 1), height - cvRound(red.at<float>(i - 1))),
             cv::Point(bin_w * (i), height - cvRound(red.at<float>(i))),
             cv::Scalar(0, 0, 255), LINE_WIDTH, 8, 0);
    cv::line(
        image,
        cv::Point(bin_w * (i - 1), height - cvRound(green.at<float>(i - 1))),
        cv::Point(bin_w * (i), height - cvRound(green.at<float>(i))),
        cv::Scalar(0, 255, 0), LINE_WIDTH, 8, 0);
    cv::line(
        image,
        cv::Point(bin_w * (i - 1), height - cvRound(blue.at<float>(i - 1))),
        cv::Point(bin_w * (i), height - cvRound(blue.at<float>(i))),
        cv::Scalar(255, 0, 0), LINE_WIDTH, 8, 0);
  }

  return image;
}

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
    auto threshold = DEFAULT_THRESHOLD;
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
