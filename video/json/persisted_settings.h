#pragma once

#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif

#include <string>
#include <map>

#if defined(WIN32)
#pragma warning(pop)
#endif

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
