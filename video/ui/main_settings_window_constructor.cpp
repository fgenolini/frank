#include "config.h"

#include "ui/main_settings_window.h"

namespace frank::video {

main_settings_window::main_settings_window(ui_controls const &controls,
                                           application_state &state,
                                           void *mock_data)
    : state_(state), controls_(controls), mock_data_(mock_data) {}

} // namespace frank::video
