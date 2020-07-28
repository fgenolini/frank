#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#endif

#include <filesystem>
#include <iostream>

#if defined(WIN32)
#pragma warning(pop)
#endif

#include "add_overlay.h"

namespace fs = std::filesystem;

namespace frank::video {

cv::Mat resize_centered(cv::Mat const &input, cv::Size const &new_size,
                        cv::Scalar const &background_colour) {
  cv::Mat output;
  auto new_height =
      (double)new_size.width * ((double)input.rows / (double)input.cols);
  auto new_width =
      (double)new_size.height * ((double)input.cols / (double)input.rows);
  auto top = 0;
  auto down = 0;
  auto left = 0;
  auto right = 0;
  if (new_height <= (double)new_size.height) {
    top = (int)(((double)new_size.height - new_height) / 2.0);
    down = top;
    cv::resize(input, output, cv::Size(new_size.width, (int)new_height));
  } else {
    left = (int)(((double)new_size.width - new_width) / 2.0);
    right = left;
    cv::resize(input, output, cv::Size((int)new_width, new_size.height));
  }

  cv::copyMakeBorder(output, output, top, down, left, right,
                     cv::BORDER_CONSTANT, background_colour);
  if (output.rows != new_size.height || output.cols != new_size.width) {
    cv::resize(output, output, cv::Size(new_size.width, new_size.height));
  }

  return output;
}

cv::Mat add_overlay(double overlay_alpha, cv::String const &overlay_image,
                    cv::Mat *overlay_buffer, cv::Mat const &frame) {
  cv::Mat output;
  if (frame.empty() || !overlay_buffer || overlay_alpha <= 0.0 ||
      overlay_alpha > 1.0 || overlay_image.size() < 1 ||
      !fs::exists(overlay_image)) {
    return output;
  }

  if (overlay_buffer->empty()) {
    auto overlay = cv::imread(overlay_image);
    if (overlay.empty()) {
      return output;
    }

    *overlay_buffer = overlay;
  }

  cv::Size new_size{frame.cols, frame.rows};
  cv::Scalar background_colour{0, 0, 0};
  auto overlay_only =
      resize_centered(*overlay_buffer, new_size, background_colour);
  if (overlay_only.cols != frame.cols) {
    std::cerr << "Wrong cols for add_overlay: " << overlay_only.cols
              << " != " << frame.cols << '\n';
    return output;
  }

  if (overlay_only.rows != frame.rows) {
    std::cerr << "Wrong rows for add_overlay: " << overlay_only.rows
              << " != " << frame.rows << '\n';
    return output;
  }

  if (overlay_only.type() != frame.type()) {
    std::cerr << "Wrong type for add_overlay\n";
    return output;
  }

  auto beta = 1.0 - overlay_alpha;
  cv::Mat overlaid_image;
  cv::addWeighted(overlay_only, overlay_alpha, frame, beta, 0.0,
                  overlaid_image);
  output = overlaid_image;
  return output;
}

} // namespace frank::video
