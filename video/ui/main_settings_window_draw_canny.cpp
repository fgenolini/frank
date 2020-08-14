#include "config.h"

#include "ui/main_settings_window.h"

namespace frank::video {

void main_settings_window::draw_canny() {
  constexpr auto TRACKBAR_WIDTH = 150;
  controls_.checkbox("Use Canny Edge", &state_.use_canny);
  controls_.begin_row();
  {
    controls_.begin_column();
    {
      controls_.text(" ");
      controls_.text(" ");
      controls_.text("low:");
    }
    controls_.end_column();
    controls_.trackbar_int(TRACKBAR_WIDTH, &state_.low_threshold, 5, 150);
    controls_.begin_column();
    {
      controls_.text(" ");
      controls_.text(" ");
      controls_.text("  high:");
    }
    controls_.end_column();
    controls_.trackbar_int(TRACKBAR_WIDTH, &state_.high_threshold, 80, 300);
  }
  controls_.end_row();
}

} // namespace frank::video
