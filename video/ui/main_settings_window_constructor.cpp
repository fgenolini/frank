#include "config.h"

#include "ui/main_settings_window.h"

namespace frank::video {

main_settings_window::main_settings_window(ui_controls &controls,
                                           application_state &state,
                                           file_dialogs &dialogs)
    : state_(state), dialogs_(dialogs), controls_(controls) {}

main_settings_window::~main_settings_window() {}

} // namespace frank::video
