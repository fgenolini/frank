#include "config.h"

WARNINGS_OFF
#include <filesystem>
WARNINGS_ON

#include "file/file_dialogs.h"
#include "main_settings_window.h"

namespace fs = std::filesystem;

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void main_settings_window::draw_overlay(
    int webcam, std::vector<cv::String> &overlay_images) {
  constexpr auto ALPHA_MAX = 1.0;
  constexpr auto ALPHA_MIN = 0.0;
  constexpr auto BUTTON_HEIGHT = 30;
  constexpr auto BUTTON_WIDTH = 70;
#if _WIN32
  constexpr auto DEFAULT_PATH = "C:\\";
#else
  constexpr auto DEFAULT_PATH = "/tmp";
#endif
  constexpr auto TRACKBAR_WIDTH = 100;

  controls_.begin_row();
  {
    controls_.begin_column();
    {
      controls_.text(" ");
      auto select_file =
          controls_.button(BUTTON_WIDTH, BUTTON_HEIGHT, "Overlay...");
      if (select_file) {
        file_dialogs dialogs{};
        auto file_names = dialogs.open_file(
            "Choose image file to use as overlay", DEFAULT_PATH,
            {"JPEG Files (.jpg .jpeg)", "*.jpg *.jpeg", "All Files", "*"});
        std::cout << "Selected files:";
        for (auto const &name : file_names) {
          if (name.empty())
            continue;
          std::cout << " " + name;
          overlay_images[webcam] = name;
          break;
        }

        std::cout << '\n';
      }
    }
    controls_.end_column();
    controls_.text(" ");
    controls_.begin_column();
    {
      controls_.text(" ");
      controls_.text(" ");
      cv::String overlay_name{};
      if ((int)overlay_images.size() <= webcam ||
          overlay_images[webcam].empty())
        overlay_name = "No overlay " + std::to_string(webcam);
      else
        overlay_name = fs::path(overlay_images[webcam]).stem().string();

      if (overlay_enabled_array_)
        controls_.checkbox(overlay_name, &overlay_enabled_array_[webcam]);
    }
    controls_.end_column();
    if (overlay_alpha_array_)
      controls_.trackbar_double(TRACKBAR_WIDTH, &overlay_alpha_array_[webcam],
                                ALPHA_MIN, ALPHA_MAX);
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
