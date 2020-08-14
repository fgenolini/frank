#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "device/input_device.h"
#include "opencv/opencv_window.h"
#include "ui/application_state.h"
#include "ui/user_interface.h"
#include "json/persisted_settings.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4820)
class video_gui : public virtual user_interface {
public:
  video_gui(int webcam_count, cvui_init const &initialise_windows,
            void *mock_data = nullptr);
  ~video_gui();

  void loop(std::vector<input_device> const &connected_webcams) override;

private:
  void load_settings();
  void save_settings();
  bool settings_changed() const;

  application_state state_{MAXIMUM_VIDEO_COUNT};
  EnhancedWindow settings_;
  opencv_window window_template_;
  persisted_settings serialiser_{};
  std::vector<bool> histograms_{};
  std::vector<cv::Mat> overlay_buffers_{};
  std::vector<cv::String> overlay_images_last_{};
  std::vector<cv::String> window_names_{};
  std::vector<double> overlay_alpha_last_{};
  std::vector<EnhancedWindow> statistics_{};
  std::vector<int> histogram_threshold_{};
  std::vector<std::pair<double, double>> height_width_pairs_{};
  std::vector<std::unique_ptr<cv::VideoCapture>> input_video_devices_{};
  void *mock_data_;
};
WARNINGS_ON

} // namespace frank::video
