
#pragma once

#include <memory>

#include <gtkmm/stack.h>
#include <gtkmm/window.h>

#include "clock.h"
#include "digital.h"
#include "gtkmm/enums.h"
#include "mopidy.h"

class PenduleWindow : public Gtk::Window {
public:
  PenduleWindow(bool fullscreen)
      : mopidy_client{new MopidyClient{"192.168.1.24", "80"}} {
    this->set_title("Pendule");

    stack.add(this->clock, "analog_page");
    stack.add(this->digital, "digital_page");
    stack.set_visible(true);
    stack.set_transition_duration(2000);
    stack.set_transition_type(Gtk::StackTransitionType::CROSSFADE);

    this->set_child(this->stack);
    if (fullscreen) {
      this->fullscreen();
    }

    this->mopidy_client->set_playback_state_change_handler(
        [this](PlaybackStateChange state_change) {
          this->playback_state_change_handler(state_change);
        });
    this->mopidy_client->start_listening();
  }

  ~PenduleWindow() override {
    if (this->mopidy_client) {
      this->mopidy_client->stop_listening();
    }
  }

private:
  Clock clock;
  DigitalClock digital;
  Gtk::Stack stack;

  std::unique_ptr<MopidyClient> mopidy_client;

  void playback_state_change_handler(PlaybackStateChange state_change) {
    if (state_change.new_state == PlaybackState::PLAYING) {
      this->stack.set_visible_child("digital_page");
    } else {
      this->stack.set_visible_child("analog_page");
    }
  }
};
