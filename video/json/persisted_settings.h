#pragma once

#include <string>
#include <map>

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
