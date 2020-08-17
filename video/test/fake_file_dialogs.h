#pragma once

#include "config.h"

#ifndef _FAKE_FILE_DIALOGS_
#error _FAKE_FILE_DIALOGS_ not defined
#endif

WARNINGS_OFF
#include <cstdint>
#include <string>
#include <vector>
WARNINGS_ON

// Faking portable-file-dialogs.h
namespace pfd {

enum class opt : uint8_t {
  none = 0,
  multiselect = 0x1,
  force_overwrite = 0x2,
  force_path = 0x4,
};

class open_file {
public:
  open_file(std::string const &title, std::string const &default_path,
            std::vector<std::string> const &filters, opt options);
  std::vector<std::string> result();

  static bool execute_called;
  static bool result_called;
  static std::vector<std::string> fake_result;
};

} // namespace pfd
