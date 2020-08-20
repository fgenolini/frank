#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct layout_command {
  void begin_column();
  void begin_row();
  void end_column();
  void end_row();
};

} // namespace frank::video
