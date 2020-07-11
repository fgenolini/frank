#pragma once

#include <opencv2/opencv.hpp>

namespace frank::video {

class opencv_window {
public:
  opencv_window(cv::String name, cv::VideoCapture *webcam, int webcam_index,
                bool has_webcam, bool video_enabled, bool use_canny = false,
                int low_threshold = 50, int high_threshold = 150);

  cv::String name() const;
  cv::VideoCapture *webcam() const;
  int high_threshold() const;
  int low_threshold() const;
  int webcam_index() const;
  bool has_webcam() const;
  bool use_canny() const;
  bool video_enabled() const;
  void set_high_threshold(int high_threshold);
  void set_low_threshold(int low_threshold);
  void set_use_canny(bool use_canny);

private:
  cv::String name_;
  cv::VideoCapture *webcam_;
  int high_threshold_;
  int low_threshold_;
  int webcam_index_;
  bool has_webcam_;
  bool use_canny_;
  bool video_enabled_;
};

} // namespace frank::video
