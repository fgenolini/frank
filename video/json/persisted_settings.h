#pragma once

#include <string>
#include <map>

namespace frank::video {

class persisted_settings {
public:
  void write(std::string text) const;
  void
  write_properties(std::map<std::string, std::string> const &properties) const;
  std::string read() const;
  std::map<std::string, std::string> read_properties() const;
};

} // namespace frank::video
