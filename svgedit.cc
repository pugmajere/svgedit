// File: svgedit.cc

#include "svgedit.h"
#include <iostream>

SvgEdit::SvgEdit()
    : m_VBox(Gtk::ORIENTATION_VERTICAL) {
  set_border_width(5);
  set_default_size(400, 200);

  // Build the main layout
  add(m_VBox);

  // Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_TextView);

  // Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_VBox.pack_start(m_ScrolledWindow);

  // Add buttons:
  m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

  m_ButtonBox.set_border_width(5);
  m_ButtonBox.set_spacing(5);
  m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);

  // Connect signals:
  fill_buffers();
  m_TextView.set_buffer(m_refTextBuffer1);
  show_all_children();
}

void SvgEdit::on_startup() {}

void SvgEdit::fill_buffers() {
  m_refTextBuffer1 = Gtk::TextBuffer::create();
  m_refTextBuffer1->set_text("");
}

SvgEdit::~SvgEdit() {}

void SvgEdit::load_file(const std::string &filename) {
  std::cout << "Load of " << filename << " requested." << std::endl;
  
}
