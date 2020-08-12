#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct layout_command {
  void begin_column(void *mock_data = nullptr) const;
  void begin_row(void *mock_data = nullptr) const;
  void end_column(void *mock_data = nullptr) const;
  void end_row(void *mock_data = nullptr) const;
};

} // namespace frank::video
