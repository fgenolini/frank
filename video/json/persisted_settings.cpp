#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "persisted_settings.h"

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

namespace frank::video {

constexpr auto FRANK_VIDEO_JSON = "frank_video.json";

void persisted_settings::write(std::string text) const {
  auto j = nlohmann::json::parse(text);
  std::ofstream o(FRANK_VIDEO_JSON);
  o << std::setw(4) << j << '\n';
}

void persisted_settings::write_properties(
    std::map<std::string, std::string> const &properties) const {
  nlohmann::json j{properties};
  std::ofstream o(FRANK_VIDEO_JSON);
  o << std::setw(4) << j << '\n';
}

std::string persisted_settings::read() const {
  if (!fs::exists(FRANK_VIDEO_JSON)) {
    return "";
  }

  std::ifstream i(FRANK_VIDEO_JSON);
  nlohmann::json j;
  i >> j;
  return j.dump();
}

std::map<std::string, std::string> persisted_settings::read_properties() const {
  std::map<std::string, std::string> empty{};
  if (!fs::exists(FRANK_VIDEO_JSON)) {
    return empty;
  }

  try {
    std::ifstream i(FRANK_VIDEO_JSON);
    nlohmann::json j;
    i >> j;
    auto all_properties = j.at(0).get<std::map<std::string, std::string>>();
    return all_properties;
  } catch (...) {
    return empty;
  }
}

} // namespace frank::video
