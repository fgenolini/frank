#include "config.h"

#include "ui/protected_cvui.h"

#include "opencv/opencv_window.h"
#include "opencv/paint_picture.h"
#include "ui/other_window.h"
#include "ui/statistics_window.h"

namespace frank::video {

bool first_row(bool has_webcam, bool first_time, int webcam_index,
               bool *histograms) {
  if (!has_webcam)
    return false;

  if (first_time)
    cvui::printf("Opening webcam %d...", webcam_index);
  else
    cvui::checkbox(STATISTICS_TITLE, histograms);

  return !first_time;
}

void other_window(EnhancedWindow &statistics, application_state &state,
                  opencv_window &window) {
  constexpr auto FIRST_ROW_X = 10;
  constexpr auto FIRST_ROW_Y = 10;
  constexpr auto WINDOW_HEIGHT = 180;
  constexpr auto WINDOW_WIDTH = 320;
  auto const first_time = window.first_time();
  auto const has_webcam = window.has_webcam();
  auto const high_threshold = window.high_threshold();
  auto const low_threshold = window.low_threshold();
  auto const overlay_alpha = window.overlay_alpha();
  auto const overlay_buffer = window.overlay_buffer();
  auto const overlay_enabled = window.use_overlay();
  auto const overlay_image = window.overlay_image();
  auto const use_canny = state.use_canny;
  auto const video_enabled = window.video_enabled();
  auto const webcam_index = window.webcam_index();
  auto histogram_threshold = window.histogram_threshold();
  auto histograms = window.histograms();
  auto frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  cv::Scalar background_colour{49, 52, 49};
  frame = background_colour;
  cvui::context(window.name());
  cv::Mat raw_picture{};
  auto picture =
      paint_picture(first_time, has_webcam, video_enabled, window, use_canny,
                    low_threshold, high_threshold, overlay_enabled,
                    overlay_alpha, overlay_image, overlay_buffer, &raw_picture);
  if (!picture.empty())
    frame = picture;

  cvui::beginRow(frame, FIRST_ROW_X, FIRST_ROW_Y);
  auto show_stats =
      first_row(has_webcam, first_time, webcam_index, &histograms);
  if (show_stats && histograms)
    statistics_window(statistics, frame, raw_picture, &histogram_threshold);

  cvui::endRow();
  cvui::imshow(window.name(), frame);
  window.set_histogram_threshold(histogram_threshold);
  window.set_histograms(histograms);
}

} // namespace frank::video
