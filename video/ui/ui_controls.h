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
  virtual ~ui_controls();

  virtual void begin_column();
  virtual void begin_row();
  virtual bool button(int width, int height, std::string const &label);
  virtual bool checkbox(std::string const &label, bool *returned_value);
  virtual void end_column();
  virtual void end_row();
  virtual void text(std::string const &label);
  virtual bool trackbar_double(int width, double *returned_value,
                               double minimum, double maximum);
  virtual bool trackbar_int(int width, int *returned_value, int minimum,
                            int maximum);

protected:
  button_command button_{};
  checkbox_command checkbox_{};
  layout_command layout_{};
  text_command text_{};
  trackbar_double_command trackbar_double_{};
  trackbar_int_command trackbar_int_{};
};

} // namespace frank::video
