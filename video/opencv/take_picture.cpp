#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif

#include <iostream>
#include <stdexcept>

#include <opencv2/imgcodecs.hpp>

#if defined(WIN32)
#pragma warning(pop)
#endif

#include "exit_requested.h"
#include "opencv_window.h"
#include "take_picture.h"

namespace frank::video {

void reduce_resolution(cv::VideoCapture *webcam, int webcam_index) {
  constexpr auto MAIN_INDEX = 0;
  constexpr auto MAXIMUM_HEIGHT_MAIN = 480.0;
  constexpr auto MAXIMUM_HEIGHT_OTHER = 180.0;
  auto webcam_height = webcam->get(cv::CAP_PROP_FRAME_HEIGHT);
  auto webcam_width = webcam->get(cv::CAP_PROP_FRAME_WIDTH);
  std::cout << "Resolution #" << webcam_index << " h:" << webcam_height
            << ", w:" << webcam_width << '\n';
  auto maximum_height = MAXIMUM_HEIGHT_OTHER;
  if (webcam_index == MAIN_INDEX) {
    maximum_height = MAXIMUM_HEIGHT_MAIN;
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

cv::Mat take_picture(opencv_window &window) noexcept(false) {
  constexpr auto MAXIMUM_RETRY = 6;
  cv::Mat picture{};
  auto webcam = window.webcam();
  if (!webcam) {
    return picture;
  }

  auto const webcam_index = window.webcam_index();
  auto retry = 0;
  auto webcam_height = window.height();
  auto webcam_width = window.width();
  while (!webcam->isOpened()) {
    if (retry >= MAXIMUM_RETRY) {
      return picture;
    }

    auto has_opened = webcam->open(webcam_index, cv::CAP_ANY);
    if (has_opened) {
      reduce_resolution(webcam, webcam_index);
      webcam_height = webcam->get(cv::CAP_PROP_FRAME_HEIGHT);
      webcam_width = webcam->get(cv::CAP_PROP_FRAME_WIDTH);
      window.set_height(webcam_height);
      window.set_width(webcam_width);
      break;
    } else {
      std::cout << "Could not open webcam " << webcam_index << '\n';
    }

    if (exit_requested()) {
      window.set_exit_requested(true);
      return picture;
    }

    ++retry;
  }

  *webcam >> picture;
  if (picture.rows != (int)webcam_height) {
    std::cerr << "Height #" << webcam_index << " rows:" << picture.rows
              << " != h:" << webcam_height << '\n';
    throw std::domain_error("webcam height is not the same as picture height");
  }

  if (picture.cols != (int)webcam_width) {
    std::cerr << "Width #" << webcam_index << " cols:" << picture.cols
              << " != w:" << webcam_width << '\n';
    throw std::domain_error("webcam width is not the same as picture width");
  }

  return picture;
}

} // namespace frank::video
