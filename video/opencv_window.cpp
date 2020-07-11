
#include "opencv_window.h"

namespace frank::video {

opencv_window::opencv_window(cv::String name, cv::VideoCapture *webcam,
                             int webcam_index, bool has_webcam,
                             bool video_enabled)
    : name_(name), webcam_(webcam), webcam_index_(webcam_index),
      has_webcam_(has_webcam), video_enabled_(video_enabled) {}

cv::String opencv_window::name() const { return name_; }

cv::VideoCapture *opencv_window::webcam() const { return webcam_; }

int opencv_window::webcam_index() const { return webcam_index_; }

bool opencv_window::has_webcam() const { return has_webcam_; }

bool opencv_window::video_enabled() const { return video_enabled_; }

} // namespace frank::video
