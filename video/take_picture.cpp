#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "exit_requested.h"
#include "opencv_window.h"
#include "take_picture.h"

namespace frank::video {

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

    if (exit_requested()) {
      window.set_exit_requested(true);
      return pic;
    }
  }

  *webcam >> pic;
  return pic;
}

} // namespace frank::video
