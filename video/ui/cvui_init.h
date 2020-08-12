#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct cvui_init {
  void execute(const std::string window_names[], size_t window_count,
               void *mock_data) const;
};

} // namespace frank::video
