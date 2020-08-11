#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

namespace frank::video {

struct file_dialogs {
  std::vector<std::string> open_file(std::string const &title,
                                     std::string const &default_path,
                                     std::vector<std::string> const &filters,
                                     void *mock_data = nullptr) const;
};

} // namespace frank::video
