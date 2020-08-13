#include "config.h"

#include "ui/ui_controls.h"

namespace frank::video {

ui_controls::ui_controls(void *mock_data) : mock_data_(mock_data) {}

void ui_controls::begin_column() const { layout_.begin_column(mock_data_); }

void ui_controls::begin_row() const { layout_.begin_row(mock_data_); }

bool ui_controls::button(int width, int height,
                         std::string const &label) const {
  return button_.execute(width, height, label, mock_data_);
}

bool ui_controls::checkbox(std::string const &label,
                           bool *returned_value) const {
  return checkbox_.execute(label, returned_value, mock_data_);
}

void ui_controls::end_column() const { layout_.end_column(mock_data_); }

void ui_controls::end_row() const { layout_.end_row(mock_data_); }

void ui_controls::text(std::string const &label) const {
  text_.execute(label, mock_data_);
}

bool ui_controls::trackbar_double(int width, double *returned_value,
                                  double minimum, double maximum) const {
  return trackbar_double_.execute(width, returned_value, minimum, maximum,
                                  mock_data_);
}

bool ui_controls::trackbar_int(int width, int *returned_value, int minimum,
                               int maximum) const {
  return trackbar_int_.execute(width, returned_value, minimum, maximum,
                               mock_data_);
}

} // namespace frank::video
