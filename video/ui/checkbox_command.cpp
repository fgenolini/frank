#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "protected_cvui.h"

#include "ui/checkbox_command.h"

namespace frank::video {

bool checkbox_command::execute(std::string const &label, bool *returned_value,
                               void *) const {
  return cvui::checkbox(label, returned_value);
}

} // namespace frank::video
