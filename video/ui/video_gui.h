#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "device/input_device.h"
#include "file/file_dialogs.h"
#include "opencv/opencv_window.h"
#include "ui/application_state.h"
#include "ui/ui_controls.h"
#include "ui/user_interface.h"
#include "json/persisted_settings.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4820)
class video_gui : public virtual user_interface {
public:
  video_gui(int webcam_count, cvui_init *initialise_windows,
            ui_controls &controls, file_dialogs &dialogs);
  virtual ~video_gui();

  virtual void
  loop(std::vector<input_device> const &connected_webcams) override;

private:
  void load_settings();
  bool one_iteration(int windows);
  bool other_window_iteration(int webcam);
  void save_settings();
  bool settings_changed() const;

  application_state state_{MAXIMUM_VIDEO_COUNT};
  EnhancedWindow settings_;
  file_dialogs &dialogs_;
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
  ui_controls &controls_;
};
WARNINGS_ON

} // namespace frank::video
