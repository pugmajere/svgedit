// File: main.cc

#include "application.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[]) {
  auto application = SvgEditApplication::create();

  // Start the application, showing the initial window,
  // and opening extra windows for any files that it is asked to open,
  // for instance as a command-line parameter.
  // run() will return when the last window has been closed by the user.
  const int status = application->run(argc, argv);
  return status;
}
