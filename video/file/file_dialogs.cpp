#include "config.h"

WARNINGS_OFF
#include <filesystem>

#if defined(_FAKE_FILE_DIALOGS_)
#include "test/fake_file_dialogs.h"
#else
#include "portable-file-dialogs.h"
#endif
WARNINGS_ON

#include "file/file_dialogs.h"

namespace fs = std::filesystem;

namespace frank::video {

file_dialogs::~file_dialogs() {}

std::vector<std::string>
file_dialogs::open_file(std::string const &title,
                        std::string const &default_path,
                        std::vector<std::string> const &filters) {
  std::vector<std::string> output{};
  auto f = pfd::open_file(title, default_path, filters, pfd::opt::force_path);
  auto selected_file_names = f.result();
  for (auto const &name : selected_file_names) {
    if (name.empty())
      continue;

    output.push_back(name);
  }

  return output;
}

} // namespace frank::video
