#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct trackbar_double_command {
  bool execute(int width, double *returned_value, double minimum,
               double maximum);
};

} // namespace frank::video
