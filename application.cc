#include "application.h"

#include <gtkmm/messagedialog.h>

#include <iostream>

SvgEditApplication::SvgEditApplication()
    : Gtk::Application("com.michonline.svgedit") {
  Glib::set_application_name("SvgEdit");
}

Glib::RefPtr<SvgEditApplication> SvgEditApplication::create() {
  return Glib::RefPtr<SvgEditApplication>(new SvgEditApplication());
}

void SvgEditApplication::on_startup() {
  // Call the base class's implementation:
  Gtk::Application::on_startup();

  // Create actions for menus and toolbars.
  // We can use add_action() because Gtk::Application derives from
  // Gio::ActionMap.

  // File menu:
  add_action("open",
             sigc::mem_fun(*this, &SvgEditApplication::on_menu_file_open));
  add_action("quit",
             sigc::mem_fun(*this, &SvgEditApplication::on_menu_file_quit));

  // Help menu:
  add_action("about",
             sigc::mem_fun(*this, &SvgEditApplication::on_menu_help_about));

  m_refBuilder = Gtk::Builder::create();

  // Layout the actions in a menubar and an application menu:
  Glib::ustring ui_info =
      "<interface>"
      "  <!-- menubar -->"
      "  <menu id='menu-example'>"
      "    <submenu>"
      "      <attribute name='label' translatable='yes'>_File</attribute>"
      "      <section>"
      "        <item>"
      "          <attribute name='label' translatable='yes'>Open</attribute>"
      "          <attribute name='action'>app.open</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;o</attribute>"
      "        </item>"
      "      </section>"
      "      <section>"
      "        <item>"
      "          <attribute name='label' translatable='yes'>_Quit</attribute>"
      "          <attribute name='action'>app.quit</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
      "        </item>"
      "      </section>"
      "    </submenu>"
      "    <submenu>"
      "      <attribute name='label' translatable='yes'>_Help</attribute>"
      "      <section>"
      "        <item>"
      "          <attribute name='label' translatable='yes'>_About</attribute>"
      "          <attribute name='action'>app.about</attribute>"
      "        </item>"
      "      </section>"
      "    </submenu>"
      "  </menu>"
      ""
      "</interface>";

  try {
    m_refBuilder->add_from_string(ui_info);
  } catch (const Glib::Error &ex) {
    std::cerr << "Building menus failed: " << ex.what();
  }

  // Get the menubar and the app menu, and add them to the application:
  auto object = m_refBuilder->get_object("menu-example");
  auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
  if (!(gmenu)) {
    g_warning("GMenu not found");
  } else {
    set_menubar(gmenu);
  }
}

void SvgEditApplication::on_activate() {
  // std::cout << "debug1: " << G_STRFUNC << std::endl;
  // The application has been started, so let's show a window.
  // A real application might want to reuse this window in on_open(),
  // when asked to open a file, if no changes have been made yet.
  create_edit_window();
  create_display_window();
}

void SvgEditApplication::create_edit_window() {
  auto win = new SvgEdit();
  svgedit = win;

  // Make sure that the application runs for as long this window is still open:
  add_window(*win);

  // Delete the window when it is hidden.
  // That's enough for this simple example.
  win->signal_hide().connect(sigc::bind<Gtk::Window *>(
      sigc::mem_fun(*this, &SvgEditApplication::on_window_hide), win));

  win->show_all();
}
void SvgEditApplication::create_display_window() {
  auto win = new SvgDisplay();
  svgdisplay = win;

  // Make sure that the application runs for as long this window is still open:
  add_window(*win);

  // Delete the window when it is hidden.
  // That's enough for this simple example.
  win->signal_hide().connect(sigc::bind<Gtk::Window *>(
      sigc::mem_fun(*this, &SvgEditApplication::on_window_hide), win));

  win->show_all();
}

void SvgEditApplication::on_window_hide(Gtk::Window *window) { delete window; }

void SvgEditApplication::on_menu_file_open() {
  Gtk::FileChooserDialog dialog("Please choose a file",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);

  // Add response buttons the the dialog:
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Open", Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_svg = Gtk::FileFilter::create();
  filter_svg->set_name("Svg files");
  filter_svg->add_pattern("*.svg");
  dialog.add_filter(filter_svg);

  // Show the dialog and wait for a user response:
  int result = dialog.run();

  // Handle the response:
  switch (result) {
    case (Gtk::RESPONSE_OK): {
      // Notice that this is a std::string, not a Glib::ustring.
      std::string filename = dialog.get_filename();
      std::cout << "File selected: " << filename << std::endl;
      svgedit->load_file(filename);
      svgdisplay->load_file(filename);
      break;
    }
    case (Gtk::RESPONSE_CANCEL): {
      std::cout << "Cancel clicked." << std::endl;
      break;
    }
    default: {
      std::cout << "Unexpected button clicked." << std::endl;
      break;
    }
  }
}

void SvgEditApplication::on_menu_file_quit() {
  std::cout << G_STRFUNC << std::endl;
  quit();  // Not really necessary, when Gtk::Widget::hide() is called.

  // Gio::Application::quit() will make Gio::Application::run() return,
  // but it's a crude way of ending the program. The window is not removed
  // from the application. Neither the window's nor the application's
  // destructors will be called, because there will be remaining reference
  // counts in both of them. If we want the destructors to be called, we
  // must remove the window from the application. One way of doing this
  // is to hide the window.
  std::vector<Gtk::Window *> windows = get_windows();
  for (const auto win : windows) {
    win->hide();
  }
}

void SvgEditApplication::on_menu_help_about() {
  Gtk::MessageDialog dialog("SvgEdit - A lightweight SVG Editor");
  dialog.set_secondary_text("Copyright 2021, Ryan Anderson.");

  dialog.run();
}
