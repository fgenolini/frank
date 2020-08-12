#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct button_command {
  bool execute(int width, int height, std::string const &label,
               void *mock_data = nullptr) const;
};

} // namespace frank::video
