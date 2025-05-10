#include "clock.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();

protected:
  Clock m_clock;
};

ExampleWindow::ExampleWindow()
{
  set_title("Pendule");
  set_child(m_clock);
}

int main(int argc, char** argv)
{
  auto app = Gtk::Application::create("io.github.orontee.pendule");

  return app->make_window_and_run<ExampleWindow>(argc, argv);
}
