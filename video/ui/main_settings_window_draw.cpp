#include "config.h"

#include "main_settings_window.h"

namespace frank::video {

void main_settings_window::draw(bool settings_minimised,
                                std::vector<input_device> const &input_devices,
                                std::vector<bool> &has_webcams,
                                std::vector<cv::String> &overlay_images) {
  if (settings_minimised)
    return;

  draw_canny();
  for (auto webcam = 0; webcam < (int)has_webcams.size(); ++webcam) {
    controls_.begin_row();
    {
      draw_webcam(webcam, input_devices);
      controls_.text(" ");
      draw_overlay(webcam, overlay_images);
    }
    controls_.end_row();
  }
}

} // namespace frank::video
