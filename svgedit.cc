// File: svgedit.cc

#include "svgedit.h"
#include "rapidxml_walk.h"
#include <glibmm.h>
#include <iostream>
#include <rapidxml/rapidxml.hpp>

SvgEdit::SvgEdit() : m_VBox(Gtk::ORIENTATION_VERTICAL) {
  set_border_width(5);
  set_default_size(400, 200);

  // Build the main layout
  add(m_VBox);

  // Add the TreeView, inside a ScrolledWindow, with the button underneath:
  // m_ScrolledWindow.add(m_TextView);
  m_ScrolledWindow.add(m_TreeView);

  // Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_VBox.pack_start(m_ScrolledWindow);

  // Create the TreeModel
  m_refTreeModel = Gtk::TreeStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);
  m_TreeView.append_column("ID", m_Columns.m_col_id);
  m_TreeView.append_column("Deets", m_Columns.m_col_name);
  m_TreeView.set_border_width(5);
  m_TreeView.set_grid_lines(Gtk::TreeViewGridLines::TREE_VIEW_GRID_LINES_BOTH);
  m_TreeView.set_enable_tree_lines(true);

  // Connect signals:
  m_TreeView.signal_row_activated().connect(
      sigc::mem_fun(*this, &SvgEdit::on_treeview_row_activated));

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

void SvgEdit::on_treeview_row_activated(const Gtk::TreeModel::Path &path,
                                        Gtk::TreeViewColumn * /* column */) {
  Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
  if (iter) {
    Gtk::TreeModel::Row row = *iter;
    std::cout << "Row activated: ID=" << row[m_Columns.m_col_id]
              << ", Name=" << row[m_Columns.m_col_name] << std::endl;
  }
}

void SvgEdit::load_file(const std::string &filename) {
  std::cout << "Load of " << filename << " requested." << std::endl;
  try {
    std::string contents(Glib::file_get_contents(filename));
    m_refTextBuffer1->set_text(contents);

    Gtk::TreeModel::Children::iterator iter =
        m_refTreeModel->children().begin();
    while (iter != m_refTreeModel->children().end()) {
      iter = m_refTreeModel->erase(iter);
    }

    char *m_Contents = new char[contents.length() + 1];
    strcpy(m_Contents, contents.c_str());
    m_Doc.parse<0>(m_Contents);

    // Find the "g" node type
    for (auto *node = m_Doc.first_node()->first_node(); node;
         node = node->next_sibling()) {
      std::cout << "node name = " << node->name() << std::endl;
      if (strcmp(node->name(), "g") == 0) {
        m_GNode = node;
        break;
      }
    }

    // Loop over the path nodes:
    if (m_GNode) {
      for (auto *path = m_GNode->first_node("path"); path;
           path = path->next_sibling("path")) {
        std::cout << "path node " << path->name() << std::endl;
        auto row = *(m_refTreeModel->append());
        row[m_Columns.m_col_id] = path->first_attribute("id")->value();
        row[m_Columns.m_col_name] = path->first_attribute("d")->value();
      }
    }
  } catch (const Glib::FileError &e) {
    std::cout << "Error opening " << filename << ": " << e.what() << std::endl;
  }
}
