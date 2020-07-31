#include "config.h"

WARNINGS_OFF
#include <filesystem>

#include "portable-file-dialogs.h"
WARNINGS_ON

#include "file_dialogs.h"

namespace fs = std::filesystem;

namespace frank::video {

std::vector<std::string>
file_dialogs::open_file(std::string const &title,
                        std::string const &default_path,
                        std::vector<std::string> const &filters) {
  std::vector<std::string> output{};
  auto f = pfd::open_file(title, default_path, filters, pfd::opt::force_path);
  for (auto const &name : f.result()) {
    output.push_back(name);
  }

  return output;
}

} // namespace frank::video
