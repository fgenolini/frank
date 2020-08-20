#include "config.h"

#include "ui/protected_cvui.h"

#include "opencv/opencv_window.h"
#include "opencv/paint_picture.h"
#include "ui/main_settings_window.h"
#include "ui/main_window.h"
#include "ui/statistics_window.h"

namespace frank::video {

main_window::main_window(ui_controls &controls, file_dialogs &dialogs)
    : dialogs_(dialogs), controls_(controls) {}

main_window::~main_window() {}

void main_window::draw(EnhancedWindow &settings, EnhancedWindow &statistics,
                       application_state &state, opencv_window &window) {
  constexpr auto FIRST_ROW_X = 10;
  constexpr auto FIRST_ROW_Y = 10;
  constexpr auto WINDOW_HEIGHT = 480;
  constexpr auto WINDOW_WIDTH = 640;
  auto const first_time = window.first_time();
  auto high_threshold = window.high_threshold();
  auto histogram_threshold = window.histogram_threshold();
  auto low_threshold = window.low_threshold();
  auto frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  auto overlay_buffer = window.overlay_buffer();
  auto use_canny = state.use_canny;
  cv::Scalar background_colour{0, 0, 0};
  frame = background_colour;
  controls_.context(window.name());
  cv::Mat raw_picture{};
  auto picture =
      paint_picture(first_time, state.has_webcams[0], state.video_enabled[0],
                    window, use_canny, low_threshold, high_threshold,
                    state.overlay_enabled[0], state.overlay_alpha[0],
                    state.overlay_images[0], overlay_buffer, &raw_picture);
  if (!picture.empty())
    frame = picture;

  controls_.begin_row(frame, FIRST_ROW_X, FIRST_ROW_Y);
  auto should_exit = false;
  first_row(state.has_webcams[0], window, should_exit);
  if (should_exit)
    return;

  controls_.end_row();
  settings_display(settings, frame, state);
  auto histograms = window.histograms();
  if (histograms)
    statistics_window(statistics, frame, raw_picture, &histogram_threshold);

  controls_.imshow(window.name(), frame);
  window.set_high_threshold(high_threshold);
  window.set_histogram_threshold(histogram_threshold);
  window.set_low_threshold(low_threshold);
}

void main_window::first_time_display(int webcam_index) {
  controls_.printf("Opening webcam %d...", webcam_index);
}

bool main_window::quit_selected(opencv_window &window) {
  constexpr auto QUIT = "Quit";
  auto should_exit = controls_.button(QUIT);
  if (should_exit)
    window.set_exit_requested(should_exit);

  return should_exit;
}

void main_window::first_row(bool has_webcam, opencv_window &window,
                            bool &should_exit) {
  should_exit = quit_selected(window);
  if (should_exit)
    return;

  if (!has_webcam)
    return;

  controls_.text(" ");
  auto const first_time = window.first_time();
  auto const webcam_index = window.webcam_index();
  if (first_time)
    return first_time_display(webcam_index);

  auto histograms = window.histograms();
  controls_.checkbox(STATISTICS_TITLE, &histograms);
  window.set_histograms(histograms);
}

void main_window::settings_display(EnhancedWindow &settings, cv::Mat frame,
                                   application_state &state) {
  settings.begin(frame);
  main_settings_window settings_window(controls_, state, dialogs_);
  settings_window.draw(settings.isMinimized());
  settings.end();
}

} // namespace frank::video
