#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

class cvui_init {
public:
  virtual ~cvui_init();

  virtual void execute(const std::string window_names[],
                       size_t window_count);
};

} // namespace frank::video
