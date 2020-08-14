#include "config.h"

#include "ui/protected_cvui.h"

#include "opencv/opencv_window.h"
#include "opencv/paint_picture.h"
#include "ui/main_settings_window.h"
#include "ui/main_window.h"
#include "ui/statistics_window.h"

namespace frank::video {

void first_time_display(int webcam_index) {
  cvui::printf("Opening webcam %d...", webcam_index);
}

bool quit_selected(opencv_window &window) {
  constexpr auto QUIT = "Quit";
  auto should_exit = cvui::button(QUIT);
  if (should_exit)
    window.set_exit_requested(should_exit);

  return should_exit;
}

void first_row(bool has_webcam, opencv_window &window, bool &should_exit) {
  should_exit = quit_selected(window);
  if (should_exit)
    return;

  if (!has_webcam)
    return;

  cvui::text(" ");
  auto const first_time = window.first_time();
  auto const webcam_index = window.webcam_index();
  if (first_time)
    return first_time_display(webcam_index);

  auto histograms = window.histograms();
  cvui::checkbox(STATISTICS_TITLE, &histograms);
  window.set_histograms(histograms);
}

void settings_display(EnhancedWindow &settings, cv::Mat frame,
                      ui_controls const &controls, application_state &state,
                      void *mock_data) {
  settings.begin(frame);
  main_settings_window settings_window(controls, state, mock_data);
  settings_window.draw(settings.isMinimized());
  settings.end();
}

void main_window(EnhancedWindow &settings, EnhancedWindow &statistics,
                 application_state &state, opencv_window &window,
                 void *mock_data) {
  constexpr auto FIRST_ROW_X = 10;
  constexpr auto FIRST_ROW_Y = 10;
  constexpr auto WINDOW_HEIGHT = 480;
  constexpr auto WINDOW_WIDTH = 640;
  ui_controls controls{mock_data};
  auto const first_time = window.first_time();
  auto high_threshold = window.high_threshold();
  auto histogram_threshold = window.histogram_threshold();
  auto low_threshold = window.low_threshold();
  auto frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  auto overlay_buffer = window.overlay_buffer();
  auto use_canny = window.use_canny();
  cv::Scalar background_colour{0, 0, 0};
  frame = background_colour;
  cvui::context(window.name());
  cv::Mat raw_picture{};
  auto picture =
      paint_picture(first_time, state.has_webcams[0], state.video_enabled[0],
                    window, use_canny, low_threshold, high_threshold,
                    state.overlay_enabled[0], state.overlay_alpha[0],
                    state.overlay_images[0], overlay_buffer, &raw_picture);
  if (!picture.empty())
    frame = picture;

  cvui::beginRow(frame, FIRST_ROW_X, FIRST_ROW_Y);
  auto should_exit = false;
  first_row(state.has_webcams[0], window, should_exit);
  if (should_exit)
    return;

  cvui::endRow();
  settings_display(settings, frame, controls, state, mock_data);
  auto histograms = window.histograms();
  if (histograms)
    statistics_window(statistics, frame, raw_picture, &histogram_threshold);

  cvui::imshow(window.name(), frame);
  window.set_high_threshold(high_threshold);
  window.set_histogram_threshold(histogram_threshold);
  window.set_low_threshold(low_threshold);
  window.set_use_canny(use_canny);
}

} // namespace frank::video
