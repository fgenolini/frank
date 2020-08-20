#include "config.h"

WARNINGS_OFF
#include <cstdarg>
#include <string>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "ui/text_command.h"

namespace frank::video {

void text_command::execute(std::string const &label) { cvui::text(label); }

void text_command::printf(const char *format...) {
  va_list args;
  va_start(args, format);
  cvui::printf(format, args);
  va_end(args);
}

} // namespace frank::video
