
#pragma once

#include <memory>

#include <gtkmm/window.h>
#include <gtkmm/stack.h>

#include "clock.h"
#include "cover.h"
#include "mopidy.h"

class PenduleWindow : public Gtk::Window {
public:
  PenduleWindow(bool fullscreen):
    mopidy_client{new MopidyClient{"192.168.1.24", "80"}}
  {
    this->set_title("Pendule");

    stack.add(this->clock, "clock_page");
    stack.add(this->cover, "cover_page");

    stack.set_visible(true);

    this->set_child(this->stack);
    if (fullscreen) {
      this->fullscreen();
    }

    this->mopidy_client->start_listening();
  }

  ~PenduleWindow() override {
    if (this->mopidy_client) {
      this->mopidy_client->stop_listening();
    }
  }

protected:
  Clock clock;
  Cover cover;
  Gtk::Stack stack;

  std::unique_ptr<MopidyClient> mopidy_client;
};
