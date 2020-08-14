#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "ui/text_command.h"

namespace frank::video {

void text_command::execute(std::string const &label, void *) const {
  cvui::text(label);
}

} // namespace frank::video
