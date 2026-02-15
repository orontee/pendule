#pragma once

#include "gtkmm/enums.h"
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/overlay.h>

class Cover : public Gtk::Overlay {
public:
  Cover() {
    this->vbox.append(this->label1);
    this->vbox.append(this->label2);
    this->vbox.set_valign(Gtk::Align::CENTER);
    this->vbox.set_halign(Gtk::Align::CENTER);

    this->set_labels();

    this->add_overlay(this->vbox);
    this->show();
  }

  void set_labels() {
    this->label1.set_text("Label 1");
    this->label2.set_text("Label 2");
  }

private:
  Gtk::Box vbox{Gtk::Orientation::VERTICAL};
  Gtk::Label label1;
  Gtk::Label label2;
};
