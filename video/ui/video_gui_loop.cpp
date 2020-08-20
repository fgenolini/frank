#include "config.h"

#include "opencv/exit_requested.h"
#include "ui/main_window.h"
#include "ui/other_window.h"
#include "ui/video_gui.h"

namespace frank::video {

void video_gui::loop(std::vector<input_device> const &connected_webcams) {
  load_settings();
  auto const webcams = (int)connected_webcams.size();
  auto const windows = (int)state_.has_webcams.size();
  for (auto webcam = 0; webcam < webcams && webcam < windows; ++webcam)
    state_.input_devices[(size_t)webcam] = connected_webcams[(size_t)webcam];

  auto should_exit = false;
  while (!should_exit)
    should_exit = one_iteration(windows);
}

bool video_gui::one_iteration(int windows) {
  window_template_.set_histogram_threshold(histogram_threshold_[0]);
  main_window main(controls_, dialogs_);
  main.draw(settings_, statistics_[0], state_, window_template_);
  histograms_[0] = window_template_.histograms();
  histogram_threshold_[0] = window_template_.histogram_threshold();
  save_settings();
  auto should_exit = window_template_.exit_requested() || exit_requested();
  for (auto webcam = 1; webcam < windows && !should_exit; ++webcam)
    should_exit = other_window_iteration(webcam);

  window_template_.set_first_time(false);
  return should_exit;
}

bool video_gui::other_window_iteration(int webcam) {
  if (!state_.has_webcams[(size_t)webcam])
    return false;

  cv::String window_name{window_names_[(size_t)webcam]};
  cv::VideoCapture *video_capture{input_video_devices_[(size_t)webcam].get()};
  auto first_time = window_template_.first_time();
  bool has_webcam = state_.has_webcams[(size_t)webcam];
  bool video_enabled = state_.video_enabled[(size_t)webcam];
  auto height_width_pair = height_width_pairs_[(size_t)webcam];
  opencv_window window(
      window_name, video_capture, webcam, first_time, has_webcam, video_enabled,
      height_width_pair, &overlay_buffers_[(size_t)webcam],
      state_.overlay_enabled[(size_t)webcam],
      state_.overlay_images[(size_t)webcam],
      state_.overlay_alpha[(size_t)webcam], window_template_.low_threshold(),
      window_template_.high_threshold(), histograms_[(size_t)webcam]);
  window.set_histogram_threshold(histogram_threshold_[(size_t)webcam]);
  other_window(statistics_[(size_t)webcam], state_, window);
  if (window.exit_requested())
    return true;

  height_width_pair.first = window.height();
  height_width_pair.second = window.width();
  height_width_pairs_[(size_t)webcam] = height_width_pair;
  histograms_[(size_t)webcam] = window.histograms();
  histogram_threshold_[(size_t)webcam] = window.histogram_threshold();
  return false;
}

} // namespace frank::video
