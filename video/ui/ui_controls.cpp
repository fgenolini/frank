#include "config.h"

WARNINGS_OFF
#include <cstdarg>
WARNINGS_ON

#include "ui/ui_controls.h"

namespace frank::video {

ui_controls::~ui_controls() {}

void ui_controls::begin_column() { layout_.begin_column(); }

void ui_controls::begin_row() { layout_.begin_row(); }

void ui_controls::begin_row(cv::Mat &frame, int x, int y) {
  layout_.begin_row(frame, x, y);
}

bool ui_controls::button(std::string const &label) {
  return button_.execute(label);
}

bool ui_controls::button(int width, int height, std::string const &label) {
  return button_.execute(width, height, label);
}

bool ui_controls::checkbox(std::string const &label, bool *returned_value) {
  return checkbox_.execute(label, returned_value);
}

void ui_controls::context(std::string const &window_name) {
  layout_.context(window_name);
}

void ui_controls::end_column() { layout_.end_column(); }

void ui_controls::end_row() { layout_.end_row(); }

void ui_controls::imshow(std::string const &window_name, cv::Mat image) {
  layout_.imshow(window_name, image);
}

void ui_controls::printf(const char *format...) {
  va_list args;
  va_start(args, format);
  text_.printf(format);
  va_end(args);
}

void ui_controls::text(std::string const &label) { text_.execute(label); }

bool ui_controls::trackbar_double(int width, double *returned_value,
                                  double minimum, double maximum) {
  return trackbar_double_.execute(width, returned_value, minimum, maximum);
}

bool ui_controls::trackbar_int(int width, int *returned_value, int minimum,
                               int maximum) {
  return trackbar_int_.execute(width, returned_value, minimum, maximum);
}

} // namespace frank::video
