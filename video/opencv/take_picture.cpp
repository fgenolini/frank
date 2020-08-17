#include "config.h"

WARNINGS_OFF
#include <stdexcept>
WARNINGS_ON

#include "opencv/open_webcam.h"
#include "opencv/take_picture.h"

namespace frank::video {

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
