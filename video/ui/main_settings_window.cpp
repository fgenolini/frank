#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4355 5204)
#endif

#include <filesystem>

#include "portable-file-dialogs.h"

#if defined(WIN32)
#pragma warning(pop)
#endif

#include "device/input_device.h"
#include "opencv/opencv_window.h"
#include "opencv/take_picture.h"
#include "main_settings_window.h"
#include "main_window.h"

namespace fs = std::filesystem;

namespace frank::video {

main_settings_window::main_settings_window(bool *video_enabled_array,
                                           bool *use_canny, int *low_threshold,
                                           int *high_threshold,
                                           bool *overlay_enabled_array,
                                           double *overlay_alpha_array)
    : overlay_alpha_array_(overlay_alpha_array),
      high_threshold_(high_threshold), low_threshold_(low_threshold),
      overlay_enabled_array_(overlay_enabled_array), use_canny_(use_canny),
      video_enabled_array_(video_enabled_array) {}

void main_settings_window::draw_canny() {
  constexpr auto TRACKBAR_WIDTH = 150;
  cvui::checkbox("Use Canny Edge", use_canny_);
  cvui::beginRow();
  {
    cvui::beginColumn();
    {
      cvui::text(" ");
      cvui::text(" ");
      cvui::text("low:");
    }
    cvui::endColumn();
    cvui::trackbar<int>(TRACKBAR_WIDTH, low_threshold_, 5, 150);
    cvui::beginColumn();
    {
      cvui::text(" ");
      cvui::text(" ");
      cvui::text("  high:");
    }
    cvui::endColumn();
    cvui::trackbar<int>(TRACKBAR_WIDTH, high_threshold_, 80, 300);
  }
  cvui::endRow();
}

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable : 4365)
#endif

void main_settings_window::draw_webcam(
    int webcam, std::vector<input_device> &input_devices) {
  cvui::beginColumn();
  {
    std::string video_name{};
    if ((int)input_devices.size() <= webcam) {
      video_name = "Video " + std::to_string(webcam);
    } else {
      video_name = std::to_string(webcam) + " " + input_devices[webcam].name();
    }

    cvui::text(" ");
    cvui::text(" ");
    cvui::checkbox(video_name, &video_enabled_array_[webcam]);
  }
  cvui::endColumn();
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

  cvui::beginRow();
  {
    cvui::beginColumn();
    {
      cvui::text(" ");
      auto select_file =
          cvui::button(BUTTON_WIDTH, BUTTON_HEIGHT, "Overlay...");
      if (select_file) {
        auto f = pfd::open_file(
            "Choose image file to use as overlay", DEFAULT_PATH,
            {"JPEG Files (.jpg .jpeg)", "*.jpg *.jpeg", "All Files", "*"},
            pfd::opt::force_path);
        std::cout << "Selected files:";
        for (auto const &name : f.result()) {
          if (name.empty())
            continue;
          std::cout << " " + name;
          overlay_images[webcam] = name;
          break;
        }

        std::cout << '\n';
      }
    }
    cvui::endColumn();
    cvui::text(" ");
    cvui::beginColumn();
    {
      cvui::text(" ");
      cvui::text(" ");
      cv::String overlay_name{};
      if (overlay_images[webcam].empty()) {
        overlay_name = "No overlay " + std::to_string(webcam);
      } else {
        overlay_name = fs::path(overlay_images[webcam]).stem().string();
      }

      cvui::checkbox(overlay_name, &overlay_enabled_array_[webcam]);
    }
    cvui::endColumn();
    cvui::trackbar<double>(TRACKBAR_WIDTH, &overlay_alpha_array_[webcam],
                           ALPHA_MIN, ALPHA_MAX);
    cvui::beginColumn();
    {
      cvui::text(" ");
      cvui::text(" ");
      cvui::text("alpha");
    }
    cvui::endColumn();
  }
  cvui::endRow();
}

#if defined(WIN32)
#pragma warning(pop)
#endif

void main_settings_window::draw(EnhancedWindow &settings,
                                std::vector<input_device> &input_devices,
                                std::vector<bool> &has_webcams,
                                std::vector<cv::String> &overlay_images) {
  if (settings.isMinimized()) {
    return;
  }

  draw_canny();
  for (auto webcam = 0; webcam < (int)has_webcams.size(); ++webcam) {
    cvui::beginRow();
    {
      draw_webcam(webcam, input_devices);
      cvui::text(" ");
      draw_overlay(webcam, overlay_images);
    }
    cvui::endRow();
  }
}

} // namespace frank::video