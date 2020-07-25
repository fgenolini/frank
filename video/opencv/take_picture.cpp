#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "exit_requested.h"
#include "opencv_window.h"
#include "take_picture.h"

namespace frank::video {

void reduce_resolution(cv::VideoCapture *webcam, int webcam_index) {
  constexpr auto MAXIMUM_HEIGHT_0 = 360.0;
  constexpr auto MAXIMUM_HEIGHT_OTHER = 240.0;
  auto webcam_height = webcam->get(cv::CAP_PROP_FRAME_HEIGHT);
  auto webcam_width = webcam->get(cv::CAP_PROP_FRAME_WIDTH);
  std::cout << "Resolution #" << webcam_index << " h:" << webcam_height
            << ", w:" << webcam_width << '\n';
  auto maximum_height = MAXIMUM_HEIGHT_OTHER;
  if (webcam_index == 0) {
    maximum_height = MAXIMUM_HEIGHT_0;
  }

  if (webcam_height <= maximum_height) {
    return;
  }

  auto new_height = maximum_height;
  auto ratio = webcam_height / maximum_height;
  auto new_width = webcam_width / ratio;
  std::cout << "Proposed reduced resolution h:" << new_height
            << ", w:" << new_width << '\n';
  webcam->set(cv::CAP_PROP_FRAME_HEIGHT, new_height);
  webcam->set(cv::CAP_PROP_FRAME_WIDTH, new_width);
  webcam_height = webcam->get(cv::CAP_PROP_FRAME_HEIGHT);
  webcam_width = webcam->get(cv::CAP_PROP_FRAME_WIDTH);
  std::cout << "Agreed new resolution h:" << webcam_height
            << ", w:" << webcam_width << '\n';
}

cv::Mat take_picture(opencv_window &window) {
  cv::Mat pic{};
  auto webcam = window.webcam();
  if (!webcam) {
    return pic;
  }

  auto webcam_index = window.webcam_index();
  while (!webcam->isOpened()) {
    auto has_opened = webcam->open(webcam_index, cv::CAP_ANY);
    if (!has_opened) {
      std::cout << "Could not open webcam " << webcam_index << '\n';
    }

    reduce_resolution(webcam, webcam_index);
    if (exit_requested()) {
      window.set_exit_requested(true);
      return pic;
    }
  }

  *webcam >> pic;
  return pic;
}

} // namespace frank::video
