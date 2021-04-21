#ifndef SVGEDIT_APPLICATION_H
#define SVGEDIT_APPLICATION_H

#include <gtkmm.h>

#include "svgdisplay.h"
#include "svgedit.h"

class SvgEditApplication : public Gtk::Application {
 protected:
  SvgEditApplication();

 public:
  static Glib::RefPtr<SvgEditApplication> create();

 protected:
  // Overrides of default signal handlers:
  void on_startup() override;
  void on_activate() override;

 private:
  void create_edit_window();
  void create_display_window();

  void on_window_hide(Gtk::Window *window);
  void on_menu_file_open();
  void on_menu_file_quit();
  void on_menu_help_about();

  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  SvgEdit *svgedit;
  SvgDisplay *svgdisplay;
};

#endif /* SVGEDIT_APPLICATION_H */
