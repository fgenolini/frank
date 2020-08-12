#include "config.h"

WARNINGS_OFF
#include <cassert>
#include <cstdlib>
#include <iostream>
WARNINGS_ON

#include "main_window.h"
#include "opencv/exit_requested.h"
#include "opencv/paint_histogram.h"
#include "other_window.h"
#include "statistics_window.h"
#include "video_gui.h"

namespace frank::video {

video_gui::~video_gui() {
  delete window_template;
  for (auto &video_capture : input_video_devices) {
    auto capturing_device = video_capture.get();
    capturing_device->release();
  }
}

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

constexpr auto ALPHA = "alpha_";
constexpr auto OVERLAY = "overlay_";

void video_gui::load_settings() {
  overlay_alpha_last_.clear();
  overlay_images_last_.clear();
  auto all_properties = serialiser_.read();
  if (all_properties.empty())
    return;

  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    std::string alpha_key = ALPHA + std::to_string(i);
    if (all_properties.count(alpha_key) > 0)
      overlay_alpha[i] = std::stod(all_properties[alpha_key]);

    std::string overlay_key = OVERLAY + std::to_string(i);
    if (all_properties.count(overlay_key) > 0)
      overlay_images[i] = all_properties[overlay_key];

    overlay_alpha_last_.push_back(overlay_alpha[i]);
    overlay_images_last_.push_back(overlay_images[i]);
  }
}

bool video_gui::settings_changed() const {
  if (overlay_images_last_.size() != MAXIMUM_VIDEO_COUNT ||
      overlay_alpha_last_.size() != MAXIMUM_VIDEO_COUNT)
    return true;

  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    if (overlay_alpha[i] != overlay_alpha_last_[i])
      return true;

    if (overlay_images[i].compare(overlay_images_last_[i]) != 0)
      return true;
  }

  return false;
}

void video_gui::save_settings() {
  if (!settings_changed())
    return;

  overlay_alpha_last_.clear();
  overlay_images_last_.clear();
  std::map<std::string, std::string> all_properties{};
  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    std::string alpha_key = ALPHA + std::to_string(i);
    all_properties[alpha_key] = std::to_string(overlay_alpha[i]);
    std::string overlay_key = OVERLAY + std::to_string(i);
    all_properties[overlay_key] = overlay_images[i];
    overlay_alpha_last_.push_back(overlay_alpha[i]);
    overlay_images_last_.push_back(overlay_images[i]);
  }

  serialiser_.write(all_properties);
}
WARNINGS_ON

} // namespace frank::video
