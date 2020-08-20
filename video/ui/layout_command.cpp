#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "ui/layout_command.h"

namespace frank::video {

void layout_command::begin_column() { cvui::beginColumn(); }

void layout_command::begin_row() { cvui::beginRow(); }

void layout_command::end_column() { cvui::endColumn(); }

void layout_command::end_row() { cvui::endRow(); }

} // namespace frank::video
