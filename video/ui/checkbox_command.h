#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

namespace frank::video {

struct checkbox_command {
  bool execute(std::string const &label, bool *returned_value,
               void *mock_data = nullptr) const;
};

} // namespace frank::video
