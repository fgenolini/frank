#include "config.h"

#include "protected_cvui.h"

#include "ui/trackbar_int_command.h"

namespace frank::video {

bool trackbar_int_command::execute(int width, int *returned_value, int minimum,
                                   int maximum, void *) const {
  return cvui::trackbar<int>(width, returned_value, minimum, maximum);
}

} // namespace frank::video
