#include "protected_cvui.h"

#include "opencv/opencv_window.h"
#include "opencv/paint_picture.h"
#include "other_window.h"

namespace frank::video {

void other_window(opencv_window &window) {
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
  auto const use_canny = window.use_canny();
  auto const use_overlay = window.use_overlay();
  auto const video_enabled = window.video_enabled();
  auto const webcam_index = window.webcam_index();
  auto histograms = window.histograms();
  auto frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  cv::Scalar background_colour{49, 52, 49};
  frame = background_colour;
  cvui::context(window.name());
  auto picture =
      paint_picture(first_time, has_webcam, video_enabled, window, use_canny,
                    low_threshold, high_threshold, overlay_enabled,
                    overlay_alpha, overlay_image, overlay_buffer);
  if (!picture.empty()) {
    frame = picture;
  }

  cvui::beginRow(frame, FIRST_ROW_X, FIRST_ROW_Y);
  {
    if (window.has_webcam()) {
      if (first_time) {
        cvui::printf("Opening webcam %d...", webcam_index);
      } else {
        cvui::checkbox("Stats", &histograms);
        if (histograms) {
          cvui::text(" ");
          cvui::printf("RGB histograms for window %d...", webcam_index);
        }
      }
    }
  }
  cvui::endRow();
  cvui::imshow(window.name(), frame);
  window.set_histograms(histograms);
}

} // namespace frank::video
