#include "config.h"

#include "opencv/reduce_resolution.h"

namespace frank::video {

void reduce_resolution(cv::VideoCapture *webcam, int webcam_index) {
  constexpr auto MAIN_INDEX = 0;
  constexpr auto MAXIMUM_HEIGHT_MAIN = 480.0;
  constexpr auto MAXIMUM_HEIGHT_OTHER = 180.0;
  auto webcam_height = webcam->get(cv::CAP_PROP_FRAME_HEIGHT);
  auto webcam_width = webcam->get(cv::CAP_PROP_FRAME_WIDTH);
  auto maximum_height = MAXIMUM_HEIGHT_OTHER;
  if (webcam_index == MAIN_INDEX)
    maximum_height = MAXIMUM_HEIGHT_MAIN;

  if (webcam_height <= maximum_height)
    return;

  auto proposed_height = maximum_height;
  auto ratio = webcam_height / maximum_height;
  auto proposed_width = webcam_width / ratio;
  webcam->set(cv::CAP_PROP_FRAME_HEIGHT, proposed_height);
  webcam->set(cv::CAP_PROP_FRAME_WIDTH, proposed_width);
}

} // namespace frank::video
