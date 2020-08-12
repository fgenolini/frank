#include "config.h"

#include "main_window.h"
#include "opencv/exit_requested.h"
#include "other_window.h"
#include "video_gui.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void video_gui::loop(std::vector<input_device> const &connected_webcams) {
  load_settings();
  while (true) {
    window_template->set_histogram_threshold(histogram_threshold[0]);
    main_window(settings, statistics_[0], connected_webcams, has_webcams,
                video_enabled, overlay_enabled, overlay_alpha, overlay_images,
                *window_template, mock_data_);
    histograms[0] = window_template->histograms();
    histogram_threshold[0] = window_template->histogram_threshold();
    save_settings();
    if (window_template->exit_requested())
      return;

    for (auto webcam = 1; webcam < (int)window_names.size(); ++webcam) {
      if (!has_webcams[webcam])
        continue;

      auto height_width_pair = height_width_pairs[webcam];
      opencv_window window(
          window_names[webcam], input_video_devices[webcam].get(), webcam,
          window_template->first_time(), has_webcams[webcam],
          video_enabled[webcam], height_width_pair, &overlay_buffers[webcam],
          window_template->use_canny(), overlay_enabled[webcam],
          overlay_images[webcam], overlay_alpha[webcam],
          window_template->low_threshold(), window_template->high_threshold(),
          histograms[webcam]);
      window.set_histogram_threshold(histogram_threshold[webcam]);
      other_window(statistics_[webcam], window);
      if (window.exit_requested())
        return;

      height_width_pair.first = window.height();
      height_width_pair.second = window.width();
      height_width_pairs[webcam] = height_width_pair;
      histograms[webcam] = window.histograms();
      histogram_threshold[webcam] = window.histogram_threshold();
    }

    if (exit_requested())
      return;

    window_template->set_first_time(false);
  }
}
WARNINGS_ON

} // namespace frank::video
