#include "config.h"

WARNINGS_OFF
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

std::unique_ptr<user_interface> make_user_interface(int webcam_count, void *) {
  return std::make_unique<video_gui>(webcam_count);
}

constexpr auto ALPHA = "alpha_";
constexpr auto OVERLAY = "overlay_";
constexpr auto SETTINGS_HEIGHT = 280;
constexpr auto SETTINGS_TITLE = "Settings";
constexpr auto SETTINGS_WIDTH = 600;
constexpr auto SETTINGS_X = 10;
constexpr auto SETTINGS_Y = 50;
constexpr auto STATISTICS0_HEIGHT = 400;
constexpr auto STATISTICS0_WIDTH = 500;
constexpr auto STATISTICS0_X = 110;
constexpr auto STATISTICS0_Y = 10;
constexpr auto STATISTICS_HEIGHT = 150;
constexpr auto STATISTICS_WIDTH = 200;
constexpr auto STATISTICS_X = 50;
constexpr auto STATISTICS_Y = 10;
constexpr auto WINDOW_NAME = "Frank video";
constexpr auto WINDOW1_NAME = "Frank video 1";
constexpr auto WINDOW2_NAME = "Frank video 2";
constexpr auto WINDOW3_NAME = "Frank video 3";

video_gui::video_gui(int webcam_count)
    : settings(SETTINGS_X, SETTINGS_Y, SETTINGS_WIDTH, SETTINGS_HEIGHT,
               SETTINGS_TITLE) {
  window_names.push_back(WINDOW_NAME);
  window_names.push_back(WINDOW1_NAME);
  window_names.push_back(WINDOW2_NAME);
  window_names.push_back(WINDOW3_NAME);
  for (auto webcam = 0;
       webcam < (int)window_names.size() && webcam < MAXIMUM_VIDEO_COUNT;
       ++webcam) {
    auto has_webcam = webcam < webcam_count;
    has_webcams.push_back(has_webcam);
    histogram_threshold[webcam] = DEFAULT_HISTOGRAM_THRESHOLD;
    overlay_buffers[webcam].addref();
    video_enabled[webcam] = has_webcam;
    if (webcam == 0) {
      statistics_.push_back(
          EnhancedWindow(STATISTICS0_X, STATISTICS0_Y, STATISTICS0_WIDTH,
                         STATISTICS0_HEIGHT, STATISTICS_TITLE));
    } else {
      statistics_.push_back(EnhancedWindow(STATISTICS_X, STATISTICS_Y,
                                           STATISTICS_WIDTH, STATISTICS_HEIGHT,
                                           STATISTICS_TITLE));
    }
  }

  if (!has_webcams[0]) {
    // Index 0 for OpenCV is also any default video input device, assume there
    // is at least one
    has_webcams[0] = true;
  }

  for ([[maybe_unused]] auto _ : has_webcams) {
    height_width_pairs.push_back(std::make_pair(0.0, 0.0));
    histograms.push_back(false);
    auto input_video_device = std::make_unique<cv::VideoCapture>();
    input_video_devices.push_back(std::move(input_video_device));
    overlay_images.push_back("");
  }

  constexpr auto FIRST_TIME = true;
  constexpr auto HIGH_THRESHOLD = 150;
  constexpr auto LOW_THRESHOLD = 50;
  constexpr auto NO_OVERLAY_ALPHA = 0.0;
  constexpr auto USE_CANNY = false;
  constexpr auto USE_OVERLAY = false;
  constexpr auto WEBCAM_INDEX = 0;
  cv::String overlay_image{};
  window_template = new opencv_window(
      window_names[0], input_video_devices[0].get(), WEBCAM_INDEX, FIRST_TIME,
      has_webcams[0], video_enabled[0], std::make_pair(0.0, 0.0),
      &overlay_buffers[0], USE_CANNY, USE_OVERLAY, overlay_image,
      NO_OVERLAY_ALPHA, LOW_THRESHOLD, HIGH_THRESHOLD, histograms[0]);
  cvui::init(&window_names[0], window_names.size());
}

video_gui::~video_gui() {
  delete window_template;
  for (auto &video_capture : input_video_devices) {
    auto capturing_device = video_capture.get();
    capturing_device->release();
  }
}

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
int video_gui::loop(std::vector<input_device> &connected_webcams) {
  load_settings();
  while (true) {
    window_template->set_histogram_threshold(histogram_threshold[0]);
    main_window(settings, statistics_[0], connected_webcams, has_webcams,
                video_enabled, overlay_enabled, overlay_alpha, overlay_images,
                *window_template);
    histograms[0] = window_template->histograms();
    histogram_threshold[0] = window_template->histogram_threshold();
    save_settings();
    if (window_template->exit_requested()) {
      return connected_webcams.size();
    }

    for (auto webcam = 1; webcam < (int)window_names.size(); ++webcam) {
      if (!has_webcams[webcam]) {
        continue;
      }

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
      if (window.exit_requested()) {
        return connected_webcams.size();
      }

      height_width_pair.first = window.height();
      height_width_pair.second = window.width();
      height_width_pairs[webcam] = height_width_pair;
      histograms[webcam] = window.histograms();
      histogram_threshold[webcam] = window.histogram_threshold();
    }

    if (exit_requested()) {
      return connected_webcams.size();
    }

    window_template->set_first_time(false);
  }

  return connected_webcams.size();
}

void video_gui::load_settings() {
  overlay_alpha_last_.clear();
  overlay_images_last_.clear();
  auto all_properties = serialiser_.read();
  if (all_properties.empty()) {
    return;
  }

  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    std::string alpha_key = ALPHA + std::to_string(i);
    if (all_properties.count(alpha_key) > 0) {
      overlay_alpha[i] = std::stod(all_properties[alpha_key]);
    }

    std::string overlay_key = OVERLAY + std::to_string(i);
    if (all_properties.count(overlay_key) > 0) {
      overlay_images[i] = all_properties[overlay_key];
    }

    overlay_alpha_last_.push_back(overlay_alpha[i]);
    overlay_images_last_.push_back(overlay_images[i]);
  }
}

bool video_gui::settings_changed() const {
  if (overlay_images_last_.size() != MAXIMUM_VIDEO_COUNT ||
      overlay_alpha_last_.size() != MAXIMUM_VIDEO_COUNT) {
    return true;
  }

  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    if (overlay_alpha[i] != overlay_alpha_last_[i]) {
      return true;
    }

    if (overlay_images[i].compare(overlay_images_last_[i]) != 0) {
      return true;
    }
  }

  return false;
}

void video_gui::save_settings() {
  if (!settings_changed()) {
    return;
  }

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
