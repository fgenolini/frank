#include "config.h"

WARNINGS_OFF
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <nlohmann/json.hpp>
WARNINGS_ON

#include "persisted_settings.h"

namespace fs = std::filesystem;

namespace frank::video {

persisted_settings::persisted_settings() {
  constexpr auto CURRENT_DIRECTORY = ".";
  constexpr auto FRANK_VIDEO_JSON = "frank_video.json";

#ifdef WIN32
  constexpr auto HOME = "USERPROFILE";
#else
  constexpr auto HOME = "HOME";
#endif

WARNINGS_OFF
  char const *home = std::getenv(HOME);
WARNINGS_ON
  if (!home) {
    home = CURRENT_DIRECTORY;
  }

  fs::path settings_file_path = home;
  settings_file_path /= FRANK_VIDEO_JSON;
  settings_file = settings_file_path.string();
}

void persisted_settings::write(
    std::map<std::string, std::string> const &properties) const {
  nlohmann::json json_properties{properties};
  std::ofstream file(settings_file);
  file << std::setw(2) << json_properties << '\n';
}

std::map<std::string, std::string> persisted_settings::read() const {
  std::map<std::string, std::string> empty{};
  if (!fs::exists(settings_file)) {
    return empty;
  }

  try {
    std::ifstream file(settings_file);
    nlohmann::json json_properties;
    file >> json_properties;
    return json_properties.at(0).get<std::map<std::string, std::string>>();
  } catch (...) {
    return empty;
  }
}

} // namespace frank::video
