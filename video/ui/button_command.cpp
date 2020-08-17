#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "ui/button_command.h"

namespace frank::video {

bool button_command::execute(int width, int height, std::string const &label) {
  return cvui::button(width, height, label);
}

} // namespace frank::video
