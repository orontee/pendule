#include "clock.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

class PenduleWindow : public Gtk::Window
{
public:
  PenduleWindow();

protected:
  Clock m_clock;
};

PenduleWindow::PenduleWindow()
{
  set_title("Pendule");
  set_child(m_clock);
  fullscreen();
}

int main(int argc, char** argv)
{
  auto app = Gtk::Application::create("io.github.orontee.pendule");

  return app->make_window_and_run<PenduleWindow>(argc, argv);
}
