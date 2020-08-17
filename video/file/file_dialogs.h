#pragma once

#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>
WARNINGS_ON

namespace frank::video {

class file_dialogs {
public:
  virtual ~file_dialogs();

  virtual std::vector<std::string>
  open_file(std::string const &title, std::string const &default_path,
            std::vector<std::string> const &filters);
};

} // namespace frank::video
