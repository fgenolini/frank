#include "config.h"

#include "main_settings_window.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void main_settings_window::draw_webcam(
    int webcam, std::vector<input_device> const &input_devices) {
  controls_.begin_column();
  {
    std::string video_name{};
    if ((int)input_devices.size() <= webcam)
      video_name = "Video " + std::to_string(webcam);
    else
      video_name = std::to_string(webcam) + " " + input_devices[webcam].name();

    controls_.text(" ");
    controls_.text(" ");
    if (video_enabled_array_)
      controls_.checkbox(video_name, &video_enabled_array_[webcam]);
  }
  controls_.end_column();
}
WARNINGS_ON

} // namespace frank::video
