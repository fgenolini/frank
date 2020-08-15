#include "config.h"

WARNINGS_OFF
#include <filesystem>

#if defined(_FAKE_FILE_DIALOGS_)
#include "test/fake_file_dialogs.h"
#else
#include "portable-file-dialogs.h"
#endif
WARNINGS_ON

#include "file_dialogs.h"

namespace fs = std::filesystem;

namespace frank::video {

std::vector<std::string>
file_dialogs::open_file(std::string const &title,
                        std::string const &default_path,
                        std::vector<std::string> const &filters,
                        void *
#if defined(_FAKE_FILE_DIALOGS_)
                            mock_data
#endif
) const {
  std::vector<std::string> output{};
#if defined(_FAKE_FILE_DIALOGS_)
  auto f = pfd::open_file(title, default_path, filters, pfd::opt::force_path,
                          mock_data);
#else
  auto f = pfd::open_file(title, default_path, filters, pfd::opt::force_path);
#endif
  for (auto const &name : f.result()) {
    if (name.empty())
      continue;

    output.push_back(name);
  }

  return output;
}

} // namespace frank::video
