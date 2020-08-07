#include "config.h"

#include "add_overlay.h"
#include "opencv_window.h"
#include "paint_picture.h"
#include "take_picture.h"

namespace frank::video {

cv::Mat paint_edges_only(cv::Mat const picture, int low_threshold,
                         int high_threshold) {
  cv::Mat canny;
  cv::cvtColor(picture, canny, cv::COLOR_BGR2GRAY);
  cv::Canny(canny, canny, low_threshold, high_threshold, 3);
  cv::cvtColor(canny, canny, cv::COLOR_GRAY2BGR);
  return canny;
}

cv::Mat paint_picture(bool first_time, bool has_webcam, bool video_enabled,
                      opencv_window &window, bool use_canny, int low_threshold,
                      int high_threshold, bool overlay_enabled,
                      double overlay_alpha, cv::String const &overlay_image,
                      cv::Mat *overlay_buffer, cv::Mat *raw_picture) {
  cv::Mat output{};
  if (first_time || !has_webcam || !video_enabled)
    return output;

  auto picture = take_picture(window);
  if (window.exit_requested() || picture.empty())
    return output;

  if (raw_picture)
    *raw_picture = picture;

  output = picture;
  if (use_canny)
    output = paint_edges_only(picture, low_threshold, high_threshold);

  if (!overlay_enabled)
    return output;

  auto with_overlay =
      add_overlay(overlay_alpha, overlay_image, overlay_buffer, output);
  if (with_overlay.empty())
    return output;

  return with_overlay;
}

} // namespace frank::video
