#include "config.h"

#include "opencv/exit_requested.h"
#include "ui/main_window.h"
#include "ui/other_window.h"
#include "ui/video_gui.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void video_gui::loop(std::vector<input_device> const &connected_webcams) {
  load_settings();
  for (auto webcam = 0; webcam < (int)connected_webcams.size(); ++webcam) {
    auto input_device = connected_webcams[webcam];
    state_.input_devices[webcam] = input_device;
  }

  while (true) {
    window_template_.set_histogram_threshold(histogram_threshold_[0]);
    main_window(settings_, statistics_[0], state_, window_template_,
                mock_data_);
    histograms_[0] = window_template_.histograms();
    histogram_threshold_[0] = window_template_.histogram_threshold();
    save_settings();
    if (window_template_.exit_requested())
      return;

    for (auto webcam = 1; webcam < (int)window_names_.size(); ++webcam) {
      if (!state_.has_webcams[webcam])
        continue;

      cv::String window_name{window_names_[webcam]};
      cv::VideoCapture *video_capture{input_video_devices_[webcam].get()};
      auto first_time = window_template_.first_time();
      bool has_webcam = state_.has_webcams[webcam];
      bool video_enabled = state_.video_enabled[webcam];
      auto height_width_pair = height_width_pairs_[webcam];
      opencv_window window(
          window_name, video_capture, webcam, first_time, has_webcam,
          video_enabled, height_width_pair, &overlay_buffers_[webcam],
          window_template_.use_canny(), state_.overlay_enabled[webcam],
          state_.overlay_images[webcam], state_.overlay_alpha[webcam],
          window_template_.low_threshold(), window_template_.high_threshold(),
          histograms_[webcam]);
      window.set_histogram_threshold(histogram_threshold_[webcam]);
      other_window(statistics_[webcam], window);
      if (window.exit_requested())
        return;

      height_width_pair.first = window.height();
      height_width_pair.second = window.width();
      height_width_pairs_[webcam] = height_width_pair;
      histograms_[webcam] = window.histograms();
      histogram_threshold_[webcam] = window.histogram_threshold();
    }

    if (exit_requested())
      return;

    window_template_.set_first_time(false);
  }
}
WARNINGS_ON

} // namespace frank::video
