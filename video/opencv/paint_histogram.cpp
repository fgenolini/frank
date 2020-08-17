#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "opencv/paint_histogram.h"

namespace frank::video {

constexpr auto HISTOGRAM_SIZE = 256;

void tone_down(int pixel_count, int threshold, cv::Mat &red, cv::Mat &green,
               cv::Mat &blue) {
  auto const most = pixel_count / threshold;
  auto const reduce_to = most / 10;
  blue.setTo(reduce_to, blue > most);
  green.setTo(reduce_to, green > most);
  red.setTo(reduce_to, red > most);
}

static cv::Scalar const red_line_colour(0, 0, 255);
static cv::Scalar const green_line_colour(0, 255, 0);
static cv::Scalar const blue_line_colour(255, 0, 0);

cv::Mat paint_histogram(int height, int width, cv::Mat const &raw_picture,
                        int tone_down_threshold) {
  constexpr auto LINE_WIDTH = 1;
  if (raw_picture.empty() || height < 1 || width < 1)
    return cv::Mat();

  cv::Mat image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
  auto const pixel_count = raw_picture.cols * raw_picture.rows;
  std::vector<cv::Mat> bgr_planes{};
  cv::split(raw_picture, bgr_planes);
  float range[] = {0.0f, 256.0f};
  const float *ranges = {range};
  auto const uniform = true;
  auto const accumulate = false;
  cv::Mat blue_hist{};
  cv::Mat green_hist{};
  cv::Mat red_hist{};
  auto const bin_count = HISTOGRAM_SIZE;
  auto bin_w = cvRound((double)width / (double)bin_count);
  cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), blue_hist, 1, &bin_count,
               &ranges, uniform, accumulate);
  cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), green_hist, 1, &bin_count,
               &ranges, uniform, accumulate);
  cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), red_hist, 1, &bin_count,
               &ranges, uniform, accumulate);
  tone_down(pixel_count, tone_down_threshold, red_hist, green_hist, blue_hist);
  cv::Mat all_colours{};
  std::vector<cv::Mat> colours = {blue_hist, green_hist, red_hist};
  cv::vconcat(colours, all_colours);
  cv::normalize(all_colours, all_colours, 0, image.rows, cv::NORM_MINMAX, -1,
                cv::Mat());
  auto const rows = all_colours.rows / 3;
  cv::Rect const blue_roi(0, 0, 1, rows);
  auto const blue = all_colours(blue_roi);
  cv::Rect const green_roi(0, rows, 1, rows);
  auto const green = all_colours(green_roi);
  cv::Rect const red_roi(0, 2 * rows, 1, rows);
  auto const red = all_colours(red_roi);
  for (auto i = 1; i < bin_count; ++i) {
    auto const pt1_x{bin_w * (i - 1)};
    auto const pt2_x{bin_w * (i)};
    cv::line(image, cv::Point(pt1_x, height - cvRound(red.at<float>(i - 1))),
             cv::Point(pt2_x, height - cvRound(red.at<float>(i))),
             red_line_colour, LINE_WIDTH, 8, 0);
    cv::line(image, cv::Point(pt1_x, height - cvRound(green.at<float>(i - 1))),
             cv::Point(pt2_x, height - cvRound(green.at<float>(i))),
             green_line_colour, LINE_WIDTH, 8, 0);
    cv::line(image, cv::Point(pt1_x, height - cvRound(blue.at<float>(i - 1))),
             cv::Point(pt2_x, height - cvRound(blue.at<float>(i))),
             blue_line_colour, LINE_WIDTH, 8, 0);
  }

  return image;
}

} // namespace frank::video
