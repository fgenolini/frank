#include "config.h"

WARNINGS_OFF
#include <iostream>
#include <stdexcept>

#include <opencv2/imgcodecs.hpp>
WARNINGS_ON

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

bool open_webcam(opencv_window &window, cv::VideoCapture *webcam,
                 int webcam_index) {
  constexpr auto MAXIMUM_RETRY = 6;

  auto has_opened = webcam->isOpened();
  if (has_opened)
    return true;

  auto retry = 0;
  auto can_continue = true;
  while (!has_opened && can_continue && retry < MAXIMUM_RETRY) {
    ++retry;
    has_opened = webcam->open(webcam_index, cv::CAP_ANY);
    if (exit_requested())
      can_continue = false;
  }

  if (!can_continue)
    window.set_exit_requested(true);

  if (!has_opened)
    return false;

  reduce_resolution(webcam, webcam_index);
  auto webcam_height = webcam->get(cv::CAP_PROP_FRAME_HEIGHT);
  auto webcam_width = webcam->get(cv::CAP_PROP_FRAME_WIDTH);
  window.set_height(webcam_height);
  window.set_width(webcam_width);
  return true;
}

cv::Mat take_picture(opencv_window &window) noexcept(false) {
  cv::Mat picture{};
  auto webcam = window.webcam();
  if (!webcam)
    return picture;

  auto const webcam_index = window.webcam_index();
  if (!open_webcam(window, webcam, webcam_index))
    return picture;

  auto webcam_height = window.height();
  auto webcam_width = window.width();
  *webcam >> picture;
  if (picture.rows != (int)webcam_height)
    throw std::domain_error("webcam height is not the same as picture height");

  if (picture.cols != (int)webcam_width)
    throw std::domain_error("webcam width is not the same as picture width");

  return picture;
}

} // namespace frank::video
