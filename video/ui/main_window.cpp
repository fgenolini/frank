#include <filesystem>
#include <memory>

#include <opencv2/opencv.hpp>

#include "../device/input_device.h"
#include "../opencv/opencv_window.h"
#include "../opencv/take_picture.h"
#include "main_window.h"
#include "portable-file-dialogs.h"

namespace fs = std::filesystem;

namespace frank::video {

void draw_canny(bool *use_canny, int *low_threshold, int *high_threshold) {
  constexpr auto TRACKBAR_WIDTH = 150;
  cvui::checkbox("Use Canny Edge", use_canny);
  cvui::beginRow();
  {
    cvui::beginColumn();
    {
      cvui::text(" ");
      cvui::text(" ");
      cvui::text("low:");
    }
    cvui::endColumn();
    cvui::trackbar<int>(TRACKBAR_WIDTH, low_threshold, 5, 150);
    cvui::beginColumn();
    {
      cvui::text(" ");
      cvui::text(" ");
      cvui::text("  high:");
    }
    cvui::endColumn();
    cvui::trackbar<int>(TRACKBAR_WIDTH, high_threshold, 80, 300);
  }
  cvui::endRow();
}

void draw_webcam(int webcam, std::vector<input_device> &input_devices,
                 bool *video_enabled_arrray) {
  cvui::beginColumn();
  {
    std::string video_name{};
    if (input_devices.size() <= webcam) {
      video_name = "Video " + std::to_string(webcam);
    } else {
      video_name = std::to_string(webcam) + " " + input_devices[webcam].name();
    }

    cvui::text(" ");
    cvui::text(" ");
    cvui::checkbox(video_name, &video_enabled_arrray[webcam]);
  }
  cvui::endColumn();
}

void draw_overlay(int webcam, bool *overlay_enabled_array,
                  double *overlay_alpha_array,
                  std::vector<cv::String> &overlay_images) {
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
    cvui::beginColumn();
    {
      cvui::text(" ");
      cvui::text(" ");
      cv::String overlay_name{};
      if (overlay_images[webcam].empty()) {
        overlay_name = "No overlay " + std::to_string(webcam);
      } else {
        overlay_name = fs::path(overlay_images[webcam]).stem();
      }

      cvui::checkbox(overlay_name, &overlay_enabled_array[webcam]);
    }
    cvui::endColumn();
    cvui::trackbar<double>(TRACKBAR_WIDTH, &overlay_alpha_array[webcam],
                           ALPHA_MIN, ALPHA_MAX);
  }
  cvui::endRow();
}

void draw_settings(EnhancedWindow &settings, bool *use_canny,
                   int *low_threshold, int *high_threshold,
                   std::vector<input_device> &input_devices,
                   std::vector<bool> &has_webcams, bool *video_enabled_arrray,
                   bool *overlay_enabled_array, double *overlay_alpha_array,
                   std::vector<cv::String> &overlay_images) {
  if (settings.isMinimized()) {
    return;
  }

  draw_canny(use_canny, low_threshold, high_threshold);
  for (auto webcam = 0; webcam < has_webcams.size(); ++webcam) {
    cvui::beginRow();
    {
      draw_webcam(webcam, input_devices, video_enabled_arrray);
      cvui::text(" ");
      draw_overlay(webcam, overlay_enabled_array, overlay_alpha_array,
                   overlay_images);
    }
    cvui::endRow();
  }
}

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled_arrray,
                 bool *overlay_enabled_array, double *overlay_alpha_array,
                 std::vector<cv::String> &overlay_images,
                 opencv_window &window) {
  constexpr auto QUIT_X = 10;
  constexpr auto QUIT_Y = 10;
  constexpr auto WINDOW_HEIGHT = 400;
  constexpr auto WINDOW_WIDTH = 700;
  auto const first_time = window.first_time();
  auto const webcam_index = window.webcam_index();
  auto high_threshold = window.high_threshold();
  auto low_threshold = window.low_threshold();
  auto main_frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  auto use_canny = window.use_canny();
  main_frame = cv::Scalar(49, 52, 49);
  cvui::context(window.name());
  if (!first_time && has_webcams[0] && video_enabled_arrray[0]) {
    auto picture = take_picture(window);
    if (window.exit_requested()) {
      return;
    }

    if (use_canny) {
      cv::cvtColor(picture, main_frame, cv::COLOR_BGR2GRAY);
      cv::Canny(main_frame, main_frame, low_threshold, high_threshold, 3);
      cv::cvtColor(main_frame, main_frame, cv::COLOR_GRAY2BGR);
    } else {
      main_frame = picture;
    }
  }

  if (first_time && has_webcams[0]) {
    cvui::printf(main_frame, 10, 10, "Opening webcam %d...", webcam_index);
  }

  auto should_exit = cvui::button(main_frame, QUIT_X, QUIT_Y, "Quit");
  if (should_exit) {
    window.set_exit_requested(true);
    return;
  }

  settings.begin(main_frame);
  draw_settings(settings, &use_canny, &low_threshold, &high_threshold,
                input_devices, has_webcams, video_enabled_arrray,
                overlay_enabled_array, overlay_alpha_array, overlay_images);
  settings.end();
  cvui::imshow(window.name(), main_frame);
  window.set_use_canny(use_canny);
  window.set_low_threshold(low_threshold);
  window.set_high_threshold(high_threshold);
}

} // namespace frank::video
