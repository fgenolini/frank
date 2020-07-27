#include <opencv2/opencv.hpp>

#include "add_overlay.h"
#include "opencv_window.h"
#include "paint_picture.h"
#include "take_picture.h"

namespace frank::video {

cv::Mat paint_picture(bool first_time, bool has_webcam, bool video_enabled,
                      opencv_window &window, bool use_canny, int low_threshold,
                      int high_threshold, bool overlay_enabled,
                      double overlay_alpha, cv::String const &overlay_image,
                      cv::Mat *overlay_buffer) {
  cv::Mat output;
  if (first_time || !has_webcam || !video_enabled) {
    return output;
  }

  auto picture = take_picture(window);
  if (window.exit_requested()) {
    return output;
  }

  cv::Mat with_canny;
  if (use_canny) {
    cv::Mat canny;
    cv::cvtColor(picture, canny, cv::COLOR_BGR2GRAY);
    cv::Canny(canny, canny, low_threshold, high_threshold, 3);
    cv::cvtColor(canny, canny, cv::COLOR_GRAY2BGR);
    with_canny = canny;
  } else {
    with_canny = picture;
  }

  if (!overlay_enabled) {
    return with_canny;
  }

  auto with_overlay =
      add_overlay(overlay_alpha, overlay_image, overlay_buffer, with_canny);
  if (!with_overlay.empty()) {
    return with_overlay;
  }

  return with_canny;
}

} // namespace frank::video
