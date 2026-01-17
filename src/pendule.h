#pragma once

#include <gtkmm/window.h>

#include "clock.h"

class PenduleWindow : public Gtk::Window {
public:
  PenduleWindow(bool fullscreen) {
    set_title("Pendule");
    set_child(this->clock);
    if (fullscreen) {
      this->fullscreen();
    }
  }

protected:
  Clock clock;
};
