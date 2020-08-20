#pragma once

#include "config.h"

WARNINGS_OFF
#include <map>
#include <string>
WARNINGS_ON

namespace frank::video {

class persisted_settings {
public:
  persisted_settings();

  void write(std::map<std::string, std::string> const &properties) const;
  std::map<std::string, std::string> read() const;

private:
  std::string settings_file;
};

} // namespace frank::video
