#pragma once

#include <opencv2/opencv.hpp>

namespace frank::video {

class opencv_window {
public:
  opencv_window(cv::String name, cv::VideoCapture *webcam, int webcam_index,
                bool has_webcam, bool video_enabled);

  cv::String name() const;
  cv::VideoCapture *webcam() const;
  int webcam_index() const;
  bool has_webcam() const;
  bool video_enabled() const;

private:
  cv::String name_;
  cv::VideoCapture *webcam_;
  int webcam_index_;
  bool has_webcam_;
  bool video_enabled_;
};

} // namespace frank::video
