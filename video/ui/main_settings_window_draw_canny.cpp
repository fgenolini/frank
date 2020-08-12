#include "config.h"

#include "main_settings_window.h"

namespace frank::video {

void main_settings_window::draw_canny() {
  constexpr auto TRACKBAR_WIDTH = 150;
  controls_.checkbox("Use Canny Edge", use_canny_);
  controls_.begin_row();
  {
    controls_.begin_column();
    {
      controls_.text(" ");
      controls_.text(" ");
      controls_.text("low:");
    }
    controls_.end_column();
    controls_.trackbar_int(TRACKBAR_WIDTH, low_threshold_, 5, 150);
    controls_.begin_column();
    {
      controls_.text(" ");
      controls_.text(" ");
      controls_.text("  high:");
    }
    controls_.end_column();
    controls_.trackbar_int(TRACKBAR_WIDTH, high_threshold_, 80, 300);
  }
  controls_.end_row();
}

} // namespace frank::video
