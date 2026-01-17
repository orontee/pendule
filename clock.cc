#include "clock.h"

#include <ctime>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include <cairomm/context.h>
#include <cairomm/fontface.h>
#include <glibmm/main.h>

namespace {
  std::string get_current_day_number() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(now, "%d");
    return oss.str();
  }
}


Clock::Clock()
: radius(0.42), line_width(0.04)
{
  Glib::signal_timeout().connect( sigc::mem_fun(*this, &Clock::on_timeout), 1000 );
  set_draw_func(sigc::mem_fun(*this, &Clock::on_draw));
}

Clock::~Clock()
{
}

void Clock::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
  // scale to unit square and translate (0, 0) to be (0.5, 0.5), i.e.
  // the center of the window
  cr->scale(width, height);
  cr->translate(0.5, 0.5);
  cr->set_line_width(this->line_width);

  this->draw_background(cr);
  this->draw_clock_face(cr);
  this->draw_date(cr);
  this->draw_hands(cr);

}

void Clock::draw_background(const Cairo::RefPtr<Cairo::Context>& cr)
{
  cr->save();
  cr->set_source_rgba(0.2, 0.0, 0.4, 0.8); // deep violet #330066
  cr->paint();
  cr->restore();
}

void Clock::draw_clock_face(const Cairo::RefPtr<Cairo::Context>& cr)
{
  cr->set_source_rgba(0.709, 0.494, 0.862, 1);   // lavender #b57edc
  cr->arc(0, 0, this->radius, 0, 2 * M_PI);
  cr->save();
  cr->set_source_rgba(0.294, 0.0, 0.509, 0.9); // indigo #4b0082
  cr->fill_preserve();
  cr->restore();
  cr->stroke_preserve();
  cr->clip();

  //clock ticks
  for (int i = 0; i < 12; i++)
  {
    double inset = 0.05;

    cr->save();
    cr->set_line_cap(Cairo::Context::LineCap::ROUND);

    if(i % 3 != 0)
    {
      inset *= 0.8;
      cr->set_line_width(0.03);
    }

    cr->move_to(
      (this->radius - inset) * cos (i * M_PI / 6),
      (this->radius - inset) * sin (i * M_PI / 6));
    cr->line_to (
      this->radius * cos (i * M_PI / 6),
      this->radius * sin (i * M_PI / 6));
    cr->stroke();
    cr->restore(); /* stack-pen-size */
  }
}

void Clock::draw_date(const Cairo::RefPtr<Cairo::Context>& cr)
{
  cr->save();
  cr->select_font_face("Sans", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::NORMAL);
  cr->set_font_size(0.1);

  const auto day_number = get_current_day_number();

  Cairo::TextExtents extents;
  cr->get_text_extents(day_number, extents);
  const double text_height = extents.height;
  const double text_width = extents.width;

  cr->move_to(this->radius * 2 / 3 - text_width, text_height / 2);
  cr->show_text(day_number);
  cr->restore();
}

void Clock::draw_hands(const Cairo::RefPtr<Cairo::Context>& cr)
{
  // store the current time
  time_t rawtime;
  time(&rawtime);
  struct tm * timeinfo = localtime (&rawtime);

  // compute the angles of the indicators of our clock
  const double minutes = timeinfo->tm_min * M_PI / 30;
  const double hours = timeinfo->tm_hour * M_PI / 6;
  const double seconds= timeinfo->tm_sec * M_PI / 30;

  cr->save();
  cr->set_line_cap(Cairo::Context::LineCap::ROUND);

  // draw the seconds hand
  cr->save();
  cr->set_line_width(this->line_width / 3);
  cr->set_source_rgba(0.7, 0.7, 0.7, 0.8); // gray
  cr->move_to(0, 0);
  cr->line_to(sin(seconds) * (this->radius * 0.9),
    -cos(seconds) * (this->radius * 0.9));
  cr->stroke();
  cr->restore();

  // draw the minutes hand
  cr->set_source_rgba(0.08, 0.466, 0.517, 0.9);   // #157784
  cr->move_to(0, 0);
  cr->line_to(sin(minutes + seconds / 60) * (this->radius * 0.8),
    -cos(minutes + seconds / 60) * (this->radius * 0.8));
  cr->stroke();

  // draw the hours hand
  cr->set_source_rgba(1.0, 0.0, 1.0, 0.9);   // fuschia #ff00ff
  cr->move_to(0, 0);
  cr->line_to(sin(hours + minutes / 12.0) * (this->radius * 0.5),
    -cos(hours + minutes / 12.0) * (this->radius * 0.5));
  cr->stroke();
  cr->restore();

  // draw a little dot in the middle
  cr->arc(0, 0, this->line_width / 3.0, 0, 2 * M_PI);
  cr->fill();
}

bool Clock::on_timeout()
{
  // force our program to redraw the entire clock.
  queue_draw();
  return true;
}
