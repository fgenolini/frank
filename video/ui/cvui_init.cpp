#include "config.h"

#include "ui/protected_cvui.h"

#include "ui/cvui_init.h"

namespace frank::video {

void cvui_init::execute(const std::string window_names[], size_t window_count,
                        void *) const {
  cvui::init(&window_names[0], window_count);
}

} // namespace frank::video
