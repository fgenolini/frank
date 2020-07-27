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

class main_settings_window {
public:
  main_settings_window(bool *video_enabled_array, bool *use_canny,
                       int *low_threshold, int *high_threshold,
                       bool *overlay_enabled_array, double *overlay_alpha_array)
      : overlay_alpha_array_(overlay_alpha_array),
        high_threshold_(high_threshold), low_threshold_(low_threshold),
        overlay_enabled_array_(overlay_enabled_array), use_canny_(use_canny),
        video_enabled_array_(video_enabled_array) {}

  void draw(EnhancedWindow &settings, std::vector<input_device> &input_devices,
            std::vector<bool> &has_webcams,
            std::vector<cv::String> &overlay_images);

private:
  void draw_canny();
  void draw_webcam(int webcam, std::vector<input_device> &input_devices);
  void draw_overlay(int webcam, std::vector<cv::String> &overlay_images);

  double *overlay_alpha_array_;
  int *high_threshold_;
  int *low_threshold_;
  bool *overlay_enabled_array_;
  bool *use_canny_;
  bool *video_enabled_array_;
};

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

void main_window(EnhancedWindow &settings,
                 std::vector<input_device> &input_devices,
                 std::vector<bool> &has_webcams, bool *video_enabled_array,
                 bool *overlay_enabled_array, double *overlay_alpha_array,
                 std::vector<cv::String> &overlay_images,
                 opencv_window &window) {
  constexpr auto QUIT_X = 10;
  constexpr auto QUIT_Y = 10;
  constexpr auto WINDOW_HEIGHT = 480;
  constexpr auto WINDOW_WIDTH = 640;
  auto const first_time = window.first_time();
  auto const webcam_index = window.webcam_index();
  auto high_threshold = window.high_threshold();
  auto low_threshold = window.low_threshold();
  auto main_frame = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  auto use_canny = window.use_canny();
  main_frame = cv::Scalar(49, 52, 49);
  cvui::context(window.name());
  if (!first_time && has_webcams[0] && video_enabled_array[0]) {
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
  main_settings_window settings_window{
      video_enabled_array,   &use_canny,
      &low_threshold,        &high_threshold,
      overlay_enabled_array, overlay_alpha_array};
  settings_window.draw(settings, input_devices, has_webcams, overlay_images);
  settings.end();
  cvui::imshow(window.name(), main_frame);
  window.set_use_canny(use_canny);
  window.set_low_threshold(low_threshold);
  window.set_high_threshold(high_threshold);
}

} // namespace frank::video
