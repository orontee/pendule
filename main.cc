#include <getopt.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "clock.h"

namespace {
bool fullscreen = false;

void check_fullscreen_argument(int argc, char *argv[]) {
  int opt;

  static struct option long_options[] = {
    {"fullscreen", no_argument, 0, 'f'},
    {nullptr, 0, nullptr, 0}};
  const char *short_options = "f";

  while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
    if (opt == 'f') {
      fullscreen = true;
      break;
    }
  }
}

class PenduleWindow : public Gtk::Window
  {
  public:
    PenduleWindow() {
      set_title("Pendule");
      set_child(this->clock);
      if (::fullscreen) {
        this->fullscreen();
      }
    }

protected:
  Clock clock;
};
}

int main(int argc, char **argv) {
  check_fullscreen_argument(argc, argv);

  auto app = Gtk::Application::create("io.github.orontee.pendule");

  if (fullscreen) {
    argc = 1;                   // to hide option
  }
  return app->make_window_and_run<::PenduleWindow>(argc, argv);
}
