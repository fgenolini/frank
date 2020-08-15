#include "config.h"

WARNINGS_OFF
#include <filesystem>
WARNINGS_ON

#include "ui/main_settings_window.h"

namespace fs = std::filesystem;

namespace frank::video {

void main_settings_window::open_overlay_file(int webcam) {
#if _WIN32
  constexpr auto DEFAULT_PATH = "C:\\";
#else
  constexpr auto DEFAULT_PATH = "/tmp";
#endif
  auto file_names = dialogs_.open_file(
      "Choose image file to use as overlay", DEFAULT_PATH,
      {"JPEG Files (.jpg .jpeg)", "*.jpg *.jpeg", "All Files", "*"},
      mock_data_);
  if (file_names.size() == 0 || webcam >= (int)state_.overlay_images.size())
    return;

  state_.overlay_images[webcam] = file_names[0];
}

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void main_settings_window::draw_overlay(int webcam) {
  constexpr auto ALPHA_MAX = 1.0;
  constexpr auto ALPHA_MIN = 0.0;
  constexpr auto BUTTON_HEIGHT = 30;
  constexpr auto BUTTON_WIDTH = 70;
  constexpr auto TRACKBAR_WIDTH = 100;

  controls_.begin_row();
  {
    controls_.begin_column();
    {
      controls_.text(" ");
      auto select_file =
          controls_.button(BUTTON_WIDTH, BUTTON_HEIGHT, OVERLAY_BUTTON_LABEL);
      if (select_file)
        open_overlay_file(webcam);
    }
    controls_.end_column();
    controls_.text(" ");
    controls_.begin_column();
    {
      controls_.text(" ");
      controls_.text(" ");
      cv::String overlay_name{"No overlay " + std::to_string(webcam)};
      if (webcam < (int)state_.overlay_images.size() &&
          !state_.overlay_images[webcam].empty())
        overlay_name = fs::path(state_.overlay_images[webcam]).stem().string();

      bool overlay_enabled{};
      if (webcam < (int)state_.overlay_enabled.size())
        overlay_enabled = state_.overlay_enabled[webcam];

      controls_.checkbox(overlay_name, &overlay_enabled);
      if (webcam < (int)state_.overlay_enabled.size())
        state_.overlay_enabled[webcam] = overlay_enabled;
    }
    controls_.end_column();
    double overlay_alpha{};
    if (webcam < (int)state_.overlay_alpha.size())
      overlay_alpha = state_.overlay_alpha[webcam];

    controls_.trackbar_double(TRACKBAR_WIDTH, &overlay_alpha, ALPHA_MIN,
                              ALPHA_MAX);
    if (webcam < (int)state_.overlay_alpha.size())
      state_.overlay_alpha[webcam] = overlay_alpha;

    controls_.begin_column();
    {
      controls_.text(" ");
      controls_.text(" ");
      controls_.text("alpha");
    }
    controls_.end_column();
  }
  controls_.end_row();
}
WARNINGS_ON

} // namespace frank::video
