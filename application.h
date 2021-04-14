#ifndef SVGEDIT_APPLICATION_H
#define SVGEDIT_APPLICATION_H

#include <gtkmm.h>

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
  void create_window();

  void on_window_hide(Gtk::Window *window);
  void on_menu_file_open();
  void on_menu_file_quit();
  void on_menu_help_about();

  Glib::RefPtr<Gtk::Builder> m_refBuilder;
};

#endif /* SVGEDIT_APPLICATION_H */
