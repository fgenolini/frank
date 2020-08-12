#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "protected_cvui.h"

#include "ui/button_command.h"

namespace frank::video {

bool button_command::execute(int width, int height, std::string const &label,
                             void *) const {
  return cvui::button(width, height, label);
}

} // namespace frank::video
