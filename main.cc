// File: main.cc

#include "svgedit.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "com.michonline.svgedit");

  SvgEdit svgedit;

  //Shows the window and returns when it is closed.
  return app->run(svgedit);
}
