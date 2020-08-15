#include "config.h"

#include "ui/main_settings_window.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void main_settings_window::draw_webcam(int webcam) {
  controls_.begin_column();
  {
    std::string video_name{"Video " + std::to_string(webcam)};
    if (webcam < (int)state_.input_devices.size())
      video_name =
          std::to_string(webcam) + " " + state_.input_devices[webcam].name();

    controls_.text(" ");
    controls_.text(" ");
    bool video_enabled{};
    if (webcam < (int)state_.video_enabled.size())
      video_enabled = state_.video_enabled[webcam];

    controls_.checkbox(video_name, &video_enabled);
    if (webcam < (int)state_.video_enabled.size())
      state_.video_enabled[webcam] = video_enabled;
  }
  controls_.end_column();
}
WARNINGS_ON

} // namespace frank::video
