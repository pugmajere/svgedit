#ifndef GTKMM_SVGDISPLAY_H
#define GTKMM_SVGDISPLAY_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include "svgrender.h"

class SvgDisplay : public Gtk::Window {
 public:
  SvgDisplay();
  virtual ~SvgDisplay();
  virtual void on_startup();

  void load_file(const std::string &filename);

 protected:
  Gtk::Grid m_Container;
  SvgRenderArea renderArea_;
};

#endif
