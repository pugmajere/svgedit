// File: svgedit.cc

#include "svgedit.h"

#include <antlr4-runtime.h>
#include <glibmm.h>

#include <iostream>
#include <memory>
#include <rapidxml/rapidxml.hpp>
#include <stdexcept>
#include <string>

#include "antlr/pathBaseVisitor.h"
#include "antlr/pathLexer.h"
#include "antlr/pathParser.h"

class Point {
 public:
  Point(){};
  Point(float x, float y) : x_(x), y_(y){};

  float x_, y_;
};

class Command {
 public:
  Command(std::string command_type, std::vector<Point> points)
      : command_type_(command_type), points_(points) {}

  std::string command_type_;
  std::vector<Point> points_;
};

class Visitor : public pathBaseVisitor {
 public:
  antlrcpp::Any visitPath(pathParser::PathContext *ctx) {
    std::vector<Command> commands;
    for (auto c : ctx->command()) {
      auto vpoints = visitCommand(c);
      std::vector<Point> points = vpoints.as<std::vector<Point>>();
      Command cmd(c->command_type()->COMMAND_TYPE()->getText(), points);
      commands.push_back(cmd);
    }

    return commands;
  }

  antlrcpp::Any visitCommand(pathParser::CommandContext *ctx) {
    std::vector<Point> points;
    for (auto p : ctx->points) {
      antlrcpp::Any pt = visitPoint(p);
      points.push_back(Point(pt));
    }

    return points;
  }

  antlrcpp::Any visitPoint(pathParser::PointContext *ctx) {
    auto coords = ctx->COORD();
    Point point;
    if (coords.size() == 1) {
      auto vx = coords[0];
      std::string xstr = vx->getText();
      float x = std::stof(xstr);
      point.x_ = x;
    } else if (coords.size() == 2) {
      auto vx = coords[0];
      std::string xstr = vx->getText();
      float x = std::stof(xstr);
      auto vy = coords[1];
      std::string ystr = vy->getText();
      float y = std::stof(ystr);

      point = Point(x, y);
    }

    return point;
  }
};

// I wish antlr4 was compatible with C++20 so I could use std::format.
template <typename... Args>
std::string string_format(const std::string &format, Args... args) {
  int size_s = snprintf(nullptr, 0, format.c_str(), args...) +
               1;  // Extra space for '\0'
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

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

        // Split the path into commands
        std::istringstream stream(path->first_attribute("d")->value());
        antlr4::ANTLRInputStream input(stream);
        pathLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        pathParser parser(&tokens);
        pathParser::PathContext *tree = parser.path();

        Visitor visitor;
        std::vector<Command> commands;
        auto vcommands = visitor.visitPath(tree);
        commands = vcommands.as<std::vector<Command>>();

        row[m_Columns.m_col_name] =
            string_format("%d commands", commands.size());
        int count = 0;
        for (auto cmd : commands) {
          Gtk::TreeModel::Row cmdrow =
              *(m_refTreeModel->append(row.children()));
          cmdrow[m_Columns.m_col_id] = path->first_attribute("id")->value() +
                                       string_format("_%d", count);
          cmdrow[m_Columns.m_col_name] =
              string_format("%d points", cmd.points_.size());
          int pcount = 0;
          for (auto pt : cmd.points_) {
            Gtk::TreeModel::Row ptrow =
                *(m_refTreeModel->append(cmdrow.children()));
            ptrow[m_Columns.m_col_id] = path->first_attribute("id")->value() +
                                        string_format("_%d", count) +
                                        string_format("_%d", pcount);
            ptrow[m_Columns.m_col_name] = string_format("%f, %f", pt.x_, pt.y_);
            pcount++;
          }
          count++;
        }
      }
    }
  } catch (const Glib::FileError &e) {
    std::cout << "Error opening " << filename << ": " << e.what() << std::endl;
  } catch (const std::bad_cast &e) {
    std::cout << "bad cast " << e.what() << std::endl;
  }
}
