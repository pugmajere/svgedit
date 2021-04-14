#include "application.h"
#include "svgedit.h"
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
  create_window();
}

void SvgEditApplication::create_window() {
  auto win = new SvgEdit();

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
  std::cout << "A File|Open menu item was selected." << std::endl;
}

void SvgEditApplication::on_menu_file_quit() {
  std::cout << G_STRFUNC << std::endl;
  quit(); // Not really necessary, when Gtk::Widget::hide() is called.

  // Gio::Application::quit() will make Gio::Application::run() return,
  // but it's a crude way of ending the program. The window is not removed
  // from the application. Neither the window's nor the application's
  // destructors will be called, because there will be remaining reference
  // counts in both of them. If we want the destructors to be called, we
  // must remove the window from the application. One way of doing this
  // is to hide the window.
  std::vector<Gtk::Window *> windows = get_windows();
  if (windows.size() > 0)
    windows[0]
        ->hide(); // In this simple case, we know there is only one window.
}

void SvgEditApplication::on_menu_help_about() {
  std::cout << "App|Help|About was selected." << std::endl;
}
