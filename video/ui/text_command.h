#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct text_command {
  void execute(std::string const &label);
};

} // namespace frank::video
