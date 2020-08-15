#include "config.h"

#include "ui/main_settings_window.h"

namespace frank::video {

main_settings_window::main_settings_window(ui_controls const &controls,
                                           application_state &state,
                                           file_dialogs const &dialogs,
                                           void *mock_data)
    : state_(state), dialogs_(dialogs), controls_(controls),
      mock_data_(mock_data) {}

} // namespace frank::video
