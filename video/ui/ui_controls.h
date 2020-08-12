#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "ui/button_command.h"
#include "ui/checkbox_command.h"
#include "ui/layout_command.h"
#include "ui/text_command.h"
#include "ui/trackbar_double_command.h"
#include "ui/trackbar_int_command.h"

namespace frank::video {

class ui_controls {
public:
  ui_controls(void *mock_data);

  void begin_column() const;
  void begin_row() const;
  bool button(int width, int height, std::string const &label) const;
  bool checkbox(std::string const &label, bool *returned_value) const;
  void end_column() const;
  void end_row() const;
  void text(std::string const &label) const;
  bool trackbar_double(int width, double *returned_value, double minimum,
                       double maximum) const;
  bool trackbar_int(int width, int *returned_value, int minimum,
                    int maximum) const;

private:
  void *mock_data_;
  button_command button_{};
  checkbox_command checkbox_{};
  layout_command layout_{};
  text_command text_{};
  trackbar_double_command trackbar_double_{};
  trackbar_int_command trackbar_int_{};
};

} // namespace frank::video
