#include "config.h"

#include "main_settings_window.h"

namespace frank::video {

main_settings_window::main_settings_window(
    ui_controls const &controls, bool *video_enabled_array, bool *use_canny,
    int *low_threshold, int *high_threshold, bool *overlay_enabled_array,
    double *overlay_alpha_array, void *mock_data)
    : controls_(controls), overlay_alpha_array_(overlay_alpha_array),
      high_threshold_(high_threshold), low_threshold_(low_threshold),
      overlay_enabled_array_(overlay_enabled_array), use_canny_(use_canny),
      video_enabled_array_(video_enabled_array), mock_data_(mock_data) {}

} // namespace frank::video
