#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "game_resources.h"

class menu_view
{
public:
  menu_view();

  /// Run the menu, until the user quits
  void exec();

  auto get_margin_size() const noexcept { return m_margin_size; }

  auto& get_resources() noexcept { return m_resources; }

  auto& get_window() noexcept { return m_window; }

private:

  /// Size of the margin in pixels
  int m_margin_size;

  /// The window to draw to
  sf::RenderWindow m_window;

  game_resources m_resources;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

void show_panel_1(menu_view& v);
void show_panel_2(menu_view& v);
void show_panel_3(menu_view& v);
void show_panel_4(menu_view& v);

#endif // LOGIC_ONLY

#endif // MENU_VIEW_H
