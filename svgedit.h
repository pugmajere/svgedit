// File: svgedit.h

#ifndef SVGEDIT_H
#define SVGEDIT_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include <rapidxml/rapidxml.hpp>

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
  void on_treeview_row_activated(const Gtk::TreeModel::Path &path,
                                 Gtk::TreeViewColumn *column);

  // Child widgets:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord {
   public:
    ModelColumns() {
      add(m_col_id);
      add(m_col_name);
    }
    Gtk::TreeModelColumn<Glib::ustring> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };

  ModelColumns m_Columns;
  Gtk::Box m_VBox;
  Gtk::ScrolledWindow m_ScrolledWindow;

  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;

  Gtk::TextView m_TextView;

  Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;

  rapidxml::xml_document<> m_Doc;
  rapidxml::xml_node<> *m_GNode;
  char *m_Contents;
};

#endif
