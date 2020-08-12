#include "config.h"

WARNINGS_OFF
#include <filesystem>
WARNINGS_ON

#include "device/input_device.h"
#include "file/file_dialogs.h"
#include "main_settings_window.h"
#include "main_window.h"
#include "opencv/opencv_window.h"
#include "opencv/take_picture.h"

namespace fs = std::filesystem;

namespace frank::video {

main_settings_window::main_settings_window(
    button_command const &button, checkbox_command const &checkbox,
    layout_command const &layout, text_command const &text,
    trackbar_double_command const &trackbar_double,
    trackbar_int_command const &trackbar_int, bool *video_enabled_array,
    bool *use_canny, int *low_threshold, int *high_threshold,
    bool *overlay_enabled_array, double *overlay_alpha_array, void *mock_data)
    : button_(button), checkbox_(checkbox), layout_(layout), text_(text),
      trackbar_double_(trackbar_double), trackbar_int_(trackbar_int),
      overlay_alpha_array_(overlay_alpha_array),
      high_threshold_(high_threshold), low_threshold_(low_threshold),
      overlay_enabled_array_(overlay_enabled_array), use_canny_(use_canny),
      video_enabled_array_(video_enabled_array), mock_data_(mock_data) {}

void main_settings_window::draw_canny() {
  constexpr auto TRACKBAR_WIDTH = 150;
  checkbox_.execute("Use Canny Edge", use_canny_, mock_data_);
  layout_.begin_row(mock_data_);
  {
    layout_.begin_column(mock_data_);
    {
      text_.execute(" ", mock_data_);
      text_.execute(" ", mock_data_);
      text_.execute("low:", mock_data_);
    }
    layout_.end_column(mock_data_);
    trackbar_int_.execute(TRACKBAR_WIDTH, low_threshold_, 5, 150, mock_data_);
    layout_.begin_column(mock_data_);
    {
      text_.execute(" ", mock_data_);
      text_.execute(" ", mock_data_);
      text_.execute("  high:", mock_data_);
    }
    layout_.end_column(mock_data_);
    trackbar_int_.execute(TRACKBAR_WIDTH, high_threshold_, 80, 300, mock_data_);
  }
  layout_.end_row(mock_data_);
}

WARNING_PUSH
DISABLE_WARNING_MSC(4365)
void main_settings_window::draw_webcam(
    int webcam, std::vector<input_device> const &input_devices) {
  layout_.begin_column(mock_data_);
  {
    std::string video_name{};
    if ((int)input_devices.size() <= webcam)
      video_name = "Video " + std::to_string(webcam);
    else
      video_name = std::to_string(webcam) + " " + input_devices[webcam].name();

    text_.execute(" ", mock_data_);
    text_.execute(" ", mock_data_);
    if (video_enabled_array_)
      checkbox_.execute(video_name, &video_enabled_array_[webcam], mock_data_);
  }
  layout_.end_column(mock_data_);
}

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

  layout_.begin_row(mock_data_);
  {
    layout_.begin_column(mock_data_);
    {
      text_.execute(" ", mock_data_);
      auto select_file = button_.execute(BUTTON_WIDTH, BUTTON_HEIGHT,
                                         "Overlay...", mock_data_);
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
    layout_.end_column(mock_data_);
    text_.execute(" ", mock_data_);
    layout_.begin_column(mock_data_);
    {
      text_.execute(" ", mock_data_);
      text_.execute(" ", mock_data_);
      cv::String overlay_name{};
      if ((int)overlay_images.size() <= webcam ||
          overlay_images[webcam].empty())
        overlay_name = "No overlay " + std::to_string(webcam);
      else
        overlay_name = fs::path(overlay_images[webcam]).stem().string();

      if (overlay_enabled_array_)
        checkbox_.execute(overlay_name, &overlay_enabled_array_[webcam],
                          mock_data_);
    }
    layout_.end_column(mock_data_);
    if (overlay_alpha_array_)
      trackbar_double_.execute(TRACKBAR_WIDTH, &overlay_alpha_array_[webcam],
                               ALPHA_MIN, ALPHA_MAX, mock_data_);
    layout_.begin_column(mock_data_);
    {
      text_.execute(" ", mock_data_);
      text_.execute(" ", mock_data_);
      text_.execute("alpha", mock_data_);
    }
    layout_.end_column(mock_data_);
  }
  layout_.end_row(mock_data_);
}
WARNINGS_ON

void main_settings_window::draw(bool settings_minimised,
                                std::vector<input_device> const &input_devices,
                                std::vector<bool> &has_webcams,
                                std::vector<cv::String> &overlay_images) {
  if (settings_minimised)
    return;

  draw_canny();
  for (auto webcam = 0; webcam < (int)has_webcams.size(); ++webcam) {
    layout_.begin_row(mock_data_);
    {
      draw_webcam(webcam, input_devices);
      text_.execute(" ", mock_data_);
      draw_overlay(webcam, overlay_images);
    }
    layout_.end_row(mock_data_);
  }
}

} // namespace frank::video
