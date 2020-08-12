#include "config.h"

#include "protected_cvui.h"

#include "main_settings_window.h"
#include "main_window.h"
#include "opencv/opencv_window.h"
#include "opencv/paint_picture.h"
#include "statistics_window.h"

namespace frank::video {

void main_window(EnhancedWindow &settings, EnhancedWindow &statistics,
                 std::vector<input_device> const &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled_array,
                 bool *overlay_enabled_array, double *overlay_alpha_array,
                 std::vector<cv::String> &overlay_images, opencv_window &window,
                 void *mock_data) {
  constexpr auto FIRST_ROW_X = 10;
  constexpr auto FIRST_ROW_Y = 10;
  constexpr auto QUIT = "Quit";
  constexpr auto WINDOW_HEIGHT = 480;
  constexpr auto WINDOW_WIDTH = 640;
  auto const first_time = window.first_time();
  auto const webcam_index = window.webcam_index();
  auto high_threshold = window.high_threshold();
  auto histogram_threshold = window.histogram_threshold();
  auto histograms = window.histograms();
  auto low_threshold = window.low_threshold();
  auto frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  auto overlay_buffer = window.overlay_buffer();
  auto use_canny = window.use_canny();
  cv::Scalar background_colour{0, 0, 0};
  frame = background_colour;
  cvui::context(window.name());
  cv::Mat raw_picture{};
  auto picture = paint_picture(
      first_time, has_webcams[0], video_enabled_array[0], window, use_canny,
      low_threshold, high_threshold, overlay_enabled_array[0],
      overlay_alpha_array[0], overlay_images[0], overlay_buffer, &raw_picture);
  if (!picture.empty())
    frame = picture;

  cvui::beginRow(frame, FIRST_ROW_X, FIRST_ROW_Y);
  {
    auto should_exit = cvui::button(QUIT);
    if (should_exit) {
      window.set_exit_requested(true);
      return;
    }

    if (has_webcams[0]) {
      cvui::text(" ");
      if (first_time)
        cvui::printf("Opening webcam %d...", webcam_index);
      else
        cvui::checkbox(STATISTICS_TITLE, &histograms);
    }
  }
  cvui::endRow();

  settings.begin(frame);
  ui_controls controls{mock_data};
  main_settings_window settings_window(
      controls, video_enabled_array, &use_canny, &low_threshold,
      &high_threshold, overlay_enabled_array, overlay_alpha_array, mock_data);
  settings_window.draw(settings.isMinimized(), input_devices, has_webcams,
                       overlay_images);
  settings.end();
  if (histograms)
    statistics_window(statistics, frame, raw_picture, &histogram_threshold);

  cvui::imshow(window.name(), frame);
  window.set_high_threshold(high_threshold);
  window.set_histogram_threshold(histogram_threshold);
  window.set_histograms(histograms);
  window.set_low_threshold(low_threshold);
  window.set_use_canny(use_canny);
}

} // namespace frank::video
