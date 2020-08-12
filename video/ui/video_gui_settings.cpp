#include "config.h"

#include "video_gui.h"

namespace frank::video {

constexpr auto ALPHA = "alpha_";
constexpr auto OVERLAY = "overlay_";

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void video_gui::load_settings() {
  overlay_alpha_last_.clear();
  overlay_images_last_.clear();
  auto all_properties = serialiser_.read();
  if (all_properties.empty())
    return;

  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    std::string alpha_key = ALPHA + std::to_string(i);
    if (all_properties.count(alpha_key) > 0)
      overlay_alpha[i] = std::stod(all_properties[alpha_key]);

    std::string overlay_key = OVERLAY + std::to_string(i);
    if (all_properties.count(overlay_key) > 0)
      overlay_images[i] = all_properties[overlay_key];

    overlay_alpha_last_.push_back(overlay_alpha[i]);
    overlay_images_last_.push_back(overlay_images[i]);
  }
}

bool video_gui::settings_changed() const {
  if (overlay_images_last_.size() != MAXIMUM_VIDEO_COUNT ||
      overlay_alpha_last_.size() != MAXIMUM_VIDEO_COUNT)
    return true;

  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    if (overlay_alpha[i] != overlay_alpha_last_[i])
      return true;

    if (overlay_images[i].compare(overlay_images_last_[i]) != 0)
      return true;
  }

  return false;
}

void video_gui::save_settings() {
  if (!settings_changed())
    return;

  overlay_alpha_last_.clear();
  overlay_images_last_.clear();
  std::map<std::string, std::string> all_properties{};
  for (auto i = 0; i < MAXIMUM_VIDEO_COUNT; ++i) {
    std::string alpha_key = ALPHA + std::to_string(i);
    all_properties[alpha_key] = std::to_string(overlay_alpha[i]);
    std::string overlay_key = OVERLAY + std::to_string(i);
    all_properties[overlay_key] = overlay_images[i];
    overlay_alpha_last_.push_back(overlay_alpha[i]);
    overlay_images_last_.push_back(overlay_images[i]);
  }

  serialiser_.write(all_properties);
}
WARNINGS_ON

} // namespace frank::video
