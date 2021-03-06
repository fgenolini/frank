#include "config.h"

#include "ui/protected_cvui.h"

#include "ui/trackbar_int_command.h"

namespace frank::video {

bool trackbar_int_command::execute(int width, int *returned_value, int minimum,
                                   int maximum) {
  return cvui::trackbar<int>(width, returned_value, minimum, maximum);
}

} // namespace frank::video
