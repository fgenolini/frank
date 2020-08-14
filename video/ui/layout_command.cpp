#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "ui/layout_command.h"

namespace frank::video {

void layout_command::begin_column(void *) const { cvui::beginColumn(); }

void layout_command::begin_row(void *) const { cvui::beginRow(); }

void layout_command::end_column(void *) const { cvui::endColumn(); }

void layout_command::end_row(void *) const { cvui::endRow(); }

} // namespace frank::video
