#include "config.h"

#include "protected_cvui.h"

#include "ui/trackbar_double_command.h"

namespace frank::video {

bool trackbar_double_command::execute(int width, double *returned_value,
                                      double minimum, double maximum,
                                      void *) const {
  return cvui::trackbar<double>(width, returned_value, minimum, maximum);
}

} // namespace frank::video
