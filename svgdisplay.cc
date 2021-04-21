// File: svgdisplay.cc

#include "svgdisplay.h"

#include <iostream>

SvgDisplay::SvgDisplay() {
  set_title("Rendered SVG");
  set_border_width(5);
  set_default_size(400, 200);

  // Build the main layout
  add(renderArea_);
  renderArea_.show();

  show_all_children();
}

void SvgDisplay::on_startup() {}

SvgDisplay::~SvgDisplay() {}

void SvgDisplay::load_file(const std::string &filename) {
  std::cout << "Load of " << filename << " requested." << std::endl;
  renderArea_.load_file(filename);
}
