#include <gtkmm/application.h>

#include "pendule.h"

int main(int argc, char **argv) {
  bool fullscreen = false;
  auto app = Gtk::Application::create("io.github.orontee.pendule");
  app->add_main_option_entry(
      [&fullscreen](const Glib::ustring &option_name,
                    const Glib::ustring &value, bool has_value) -> bool {
        fullscreen = true;
        return true;
      },
      "fullscreen", 'f', "Display fullscreen window", {},
      Glib::OptionEntry::Flags::OPTIONAL_ARG);

  return app->make_window_and_run<PenduleWindow>(argc, argv, fullscreen);
}
