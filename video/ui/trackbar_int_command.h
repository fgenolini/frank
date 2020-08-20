#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct trackbar_int_command {
  bool execute(int width, int *returned_value, int minimum, int maximum);
};

} // namespace frank::video
