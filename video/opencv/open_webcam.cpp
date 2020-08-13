#include "config.h"

#include "exit_requested.h"
#include "open_webcam.h"
#include "reduce_resolution.h"

namespace frank::video {

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

} // namespace frank::video
