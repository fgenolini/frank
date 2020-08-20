#include "config.h"

#include "ui/ui_controls.h"

namespace frank::video {

ui_controls::~ui_controls() {}

void ui_controls::begin_column() { layout_.begin_column(); }

void ui_controls::begin_row() { layout_.begin_row(); }

bool ui_controls::button(int width, int height, std::string const &label) {
  return button_.execute(width, height, label);
}

bool ui_controls::checkbox(std::string const &label, bool *returned_value) {
  return checkbox_.execute(label, returned_value);
}

void ui_controls::end_column() { layout_.end_column(); }

void ui_controls::end_row() { layout_.end_row(); }

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
