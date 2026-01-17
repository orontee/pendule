#pragma once

#include <cairomm/refptr.h>
#include <gtkmm/drawingarea.h>

class Clock : public Gtk::DrawingArea {
public:
  Clock();
  virtual ~Clock();

protected:
  void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);

  bool on_timeout();

  double radius;
  double line_width;

private:
  void draw_background(const Cairo::RefPtr<Cairo::Context> &cr);
  void draw_clock_face(const Cairo::RefPtr<Cairo::Context> &cr);
  void draw_date(const Cairo::RefPtr<Cairo::Context> &cr);
  void draw_hands(const Cairo::RefPtr<Cairo::Context> &cr);
};
