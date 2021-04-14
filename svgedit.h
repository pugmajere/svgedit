// File: svgedit.h

#ifndef GTKMM_EXAMPLE_SVGEDIT_H
#define GTKMM_EXAMPLE_SVGEDIT_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>

class SvgEdit : public Gtk::Window
{

public:
  SvgEdit();
  virtual ~SvgEdit();

protected:
  //Signal handlers:
  void on_button_clicked();

  //Member widgets:
  Gtk::Button m_button;
};

#endif
