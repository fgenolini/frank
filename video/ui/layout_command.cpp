#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "ui/layout_command.h"

namespace frank::video {

void layout_command::begin_column() { cvui::beginColumn(); }

void layout_command::begin_row() { cvui::beginRow(); }

void layout_command::begin_row(cv::Mat &frame, int x, int y) {
  cvui::beginRow(frame, x, y);
}

void layout_command::context(std::string const &window_name) {
  cvui::context(window_name);
}

void layout_command::end_column() { cvui::endColumn(); }

void layout_command::end_row() { cvui::endRow(); }

void layout_command::imshow(std::string const &window_name, cv::Mat image) {
  cvui::imshow(window_name, image);
}

} // namespace frank::video
