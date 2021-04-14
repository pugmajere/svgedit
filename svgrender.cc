#include "svgrender.h"
#include <cairomm/context.h>
#include <iostream>

SvgRenderArea::SvgRenderArea(): rsvgh_(NULL)
{
}

SvgRenderArea::~SvgRenderArea()
{
}

bool SvgRenderArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if (rsvgh_) {
    auto success = rsvg_handle_render_cairo(rsvgh_, cr->cobj());
    if (!success) {
      std::cout << "Error rendering SVG" << std::endl;
      return false;
    }
  }
  return true;
}

void SvgRenderArea::force_redraw() {
  auto win = get_window();
  if (win)
  {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
    win->invalidate_rect(r, false);
  }
}

void SvgRenderArea::load_file(const std::string &filename) {
  GError *err = NULL;
  if (rsvgh_) {
    free(rsvgh_);
  }
  rsvgh_ = rsvg_handle_new_from_file(filename.c_str(), &err);
  if (err) {
    std::cout << "error loading file " << (*err).message << std::endl;
    if (rsvgh_) {
      free(rsvgh_);
    }
  }
  if (!rsvgh_) {
    std::cout << "some error" << std::endl;
  }
  return;
};
