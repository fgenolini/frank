#pragma once

#include <opencv2/opencv.hpp>

namespace frank::video {

class opencv_window {
public:
  opencv_window(cv::String name, cv::VideoCapture *webcam, int webcam_index,
                bool first_time, bool has_webcam, bool video_enabled,
                bool use_canny = false, bool use_overlay = false,
                cv::String overlay_image = nullptr, double overlay_alpha = 0.0,
                int low_threshold = 50, int high_threshold = 150);

  cv::String name() const;
  cv::String overlay_image() const;
  cv::VideoCapture *webcam() const;
  double overlay_alpha() const;
  int high_threshold() const;
  int low_threshold() const;
  int webcam_index() const;
  bool exit_requested() const;
  bool first_time() const;
  bool has_webcam() const;
  bool use_canny() const;
  bool use_overlay() const;
  bool video_enabled() const;
  void set_exit_requested(bool exit_requested);
  void set_first_time(bool first_time);
  void set_high_threshold(int high_threshold);
  void set_low_threshold(int low_threshold);
  void set_overlay_alpha(double overlay_alpha);
  void set_overlay_image(cv::String overlay_image);
  void set_use_canny(bool use_canny);
  void set_use_overlay(bool use_overlay);

private:
  cv::String name_;
  cv::String overlay_image_;
  cv::VideoCapture *webcam_;
  double overlay_alpha_;
  int high_threshold_;
  int low_threshold_;
  int webcam_index_;
  bool exit_requested_;
  bool first_time_;
  bool has_webcam_;
  bool use_canny_;
  bool use_overlay_;
  bool video_enabled_;
};

} // namespace frank::video