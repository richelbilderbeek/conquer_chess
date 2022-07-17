#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>


class menu_view
{
public:
  menu_view();

  /// Run the menu, until the user quits
  void exec();

private:
  /// The window to draw to
  sf::RenderWindow m_window;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

#endif // LOGIC_ONLY

#endif // MENU_VIEW_H
