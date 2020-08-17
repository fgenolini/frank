#include "config.h"

#include "ui/ui.h"

namespace frank::video {

ui::ui(std::vector<input_device> const &connected_webcams,
       cvui_init *initialise_windows, user_interface_factory *make_ui,
       exiter *injected_exiter)
    : initialise_windows_(initialise_windows), exiter_(injected_exiter),
      connected_webcams_(connected_webcams), make_ui_(make_ui) {}

ui::~ui() {}

void ui::interface_with_user() {
  user_interface_factory default_factory{};
  auto make_ui = make_ui_ ? make_ui_ : &default_factory;
  auto ui = make_ui->make((int)connected_webcams_.size(), initialise_windows_);
  ui->loop(connected_webcams_);
}

NO_RETURN void ui::run() {
  interface_with_user();
  exiter default_exiter{};
  auto e = exiter_ ? exiter_ : &default_exiter;
  e->exit(EXIT_SUCCESS);
}

} // namespace frank::video
