// File: svgedit.h

#ifndef GTKMM_SVGEDIT_H
#define GTKMM_SVGEDIT_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class SvgEdit : public Gtk::ApplicationWindow {

public:
  SvgEdit();
  virtual ~SvgEdit();
  virtual void on_startup();

  void load_file(const std::string &filename);

protected:
  void fill_buffers();

  // Signal handlers:
  void on_button_quit();
  void on_button_buffer1();

  // Child widgets:
  Gtk::Box m_VBox;

  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TextView m_TextView;

  Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;

  Gtk::ButtonBox m_ButtonBox;
  Gtk::Button m_Button_Quit, m_Button_Buffer1;
};

#endif
