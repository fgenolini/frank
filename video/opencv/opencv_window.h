#pragma once

#include "config.h"

WARNINGS_OFF
#include <utility>

#include <opencv2/opencv.hpp>
WARNINGS_ON

namespace frank::video {

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

class opencv_window {
public:
  opencv_window(cv::String name, cv::VideoCapture *webcam, int webcam_index,
                bool first_time, bool has_webcam, bool video_enabled,
                std::pair<double, double> const &height_width,
                cv::Mat *overlay_buffer, bool use_canny = false,
                bool use_overlay = false, cv::String overlay_image = nullptr,
                double overlay_alpha = 0.0, int low_threshold = 50,
                int high_threshold = 150);

  cv::Mat *overlay_buffer() const;
  cv::String name() const;
  cv::String overlay_image() const;
  cv::VideoCapture *webcam() const;
  double height() const;
  double overlay_alpha() const;
  double width() const;
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
  void set_height(double height);
  void set_high_threshold(int high_threshold);
  void set_low_threshold(int low_threshold);
  void set_overlay_alpha(double overlay_alpha);
  void set_overlay_buffer(cv::Mat *overlay_buffer);
  void set_overlay_image(cv::String const &overlay_image);
  void set_use_canny(bool use_canny);
  void set_use_overlay(bool use_overlay);
  void set_width(double width);

private:
  cv::Mat *overlay_buffer_;
  cv::String name_;
  cv::String overlay_image_;
  cv::VideoCapture *webcam_;
  double height_;
  double overlay_alpha_;
  double width_;
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

#if defined(WIN32)
#pragma warning(pop)
#endif

} // namespace frank::video
