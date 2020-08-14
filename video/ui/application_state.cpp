#include "config.h"

#include "ui/application_state.h"

namespace frank::video {

application_state::application_state() {}

application_state::application_state(int webcam_count) {
  has_webcams.push_back(true);
  for (auto i = 1; i < webcam_count; ++i) {
    has_webcams.push_back(false);
  }

  for (auto i = 0; i < webcam_count; ++i) {
    overlay_enabled.push_back(false);
    video_enabled.push_back(false);
    overlay_alpha.push_back(0.0);
    input_devices.push_back(input_device());
    overlay_images.push_back("");
  }
}

} // namespace frank::video
