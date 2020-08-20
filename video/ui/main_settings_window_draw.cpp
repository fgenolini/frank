#include "config.h"

#include "ui/main_settings_window.h"

namespace frank::video {

void main_settings_window::for_webcam(int webcam) {
  controls_.begin_row();
  {
    draw_webcam(webcam);
    controls_.text(" ");
    draw_overlay(webcam);
  }
  controls_.end_row();
}

void main_settings_window::draw(bool settings_minimised) {
  if (settings_minimised)
    return;

  draw_canny();
  auto has_webcams_size = (int)state_.has_webcams.size();
  for (auto webcam = 0; webcam < has_webcams_size; ++webcam)
    for_webcam(webcam);
}

} // namespace frank::video
