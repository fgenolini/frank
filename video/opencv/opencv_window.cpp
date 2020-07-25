#include "opencv_window.h"

namespace frank::video {

opencv_window::opencv_window(cv::String name, cv::VideoCapture *webcam,
                             int webcam_index, bool first_time, bool has_webcam,
                             bool video_enabled, bool use_canny,
                             bool use_overlay, cv::String overlay_image,
                             double overlay_alpha, int low_threshold,
                             int high_threshold)
    : name_(name), webcam_(webcam), overlay_alpha_(overlay_alpha),
      high_threshold_(high_threshold), low_threshold_(low_threshold),
      webcam_index_(webcam_index), exit_requested_(false),
      first_time_(first_time), has_webcam_(has_webcam), use_canny_(use_canny),
      use_overlay_(use_overlay), overlay_image_(overlay_image),
      video_enabled_(video_enabled) {}

cv::String opencv_window::name() const { return name_; }

cv::String opencv_window::overlay_image() const { return overlay_image_; }

cv::VideoCapture *opencv_window::webcam() const { return webcam_; }

double opencv_window::overlay_alpha() const { return overlay_alpha_; }

int opencv_window::high_threshold() const { return high_threshold_; }

int opencv_window::low_threshold() const { return low_threshold_; }

int opencv_window::webcam_index() const { return webcam_index_; }

bool opencv_window::exit_requested() const { return exit_requested_; }

bool opencv_window::first_time() const { return first_time_; }

bool opencv_window::has_webcam() const { return has_webcam_; }

bool opencv_window::use_canny() const { return use_canny_; }

bool opencv_window::use_overlay() const { return use_overlay_; }

bool opencv_window::video_enabled() const { return video_enabled_; }

void opencv_window::set_exit_requested(bool exit_requested) {
  exit_requested_ = exit_requested;
}

void opencv_window::set_first_time(bool first_time) {
  first_time_ = first_time;
}

void opencv_window::set_high_threshold(int high_threshold) {
  high_threshold_ = high_threshold;
}

void opencv_window::set_low_threshold(int low_threshold) {
  low_threshold_ = low_threshold;
}

void opencv_window::set_overlay_alpha(double overlay_alpha) {
  overlay_alpha_ = overlay_alpha;
}

void opencv_window::set_overlay_image(cv::String overlay_image) {
  overlay_image_ = overlay_image;
}

void opencv_window::set_use_canny(bool use_canny) { use_canny_ = use_canny; }

void opencv_window::set_use_overlay(bool use_overlay) {
  use_overlay_ = use_overlay;
}

} // namespace frank::video