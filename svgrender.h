#ifndef GTKMM_SVGRENDER_H
#define GTKMM_SVGRENDER_H

#include <gtkmm/drawingarea.h>
#include <memory>
#include <librsvg/rsvg.h>

class SvgRenderArea : public Gtk::DrawingArea
{
public:
  SvgRenderArea();
  virtual ~SvgRenderArea();
  void force_redraw();

  void load_file(const std::string &filename);

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

 private:
  RsvgHandle *rsvgh_;
};

#endif // GTKMM_SVGRENDER_H
