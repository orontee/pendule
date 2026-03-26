#pragma once

#include <ctime>
#include <iomanip>
#include <string>

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/overlay.h>
#include <gtkmm/picture.h>
#include <glibmm/main.h>

namespace {
std::string get_current_time() {
  std::time_t t = std::time(nullptr);
  std::tm *now = std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(now, "%H:%M");
  return oss.str();
}
}

class DigitalClock : public Gtk::Overlay {
public:
  DigitalClock() {
    this->vbox.append(this->clock_label);
    this->vbox.set_valign(Gtk::Align::START);
    this->vbox.set_halign(Gtk::Align::CENTER);

    this->update_clock_label();

    this->set_child(this->cover);
    this->add_overlay(this->vbox);
    this->show();

    Glib::signal_timeout().connect(sigc::mem_fun(*this, &DigitalClock::on_timeout),
                                   60000);

  }

  ~DigitalClock() override {}

protected:
  bool on_timeout() {
    this->update_clock_label();
    return true;
  }

private:
  Gtk::Picture cover;
  Gtk::Box vbox{Gtk::Orientation::VERTICAL};
  Gtk::Label clock_label;

  void update_clock_label() {
    const auto time_string = get_current_time();
    this->clock_label.set_text(time_string);
  }
};
