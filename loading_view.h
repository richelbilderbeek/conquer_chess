#ifndef LOADING_VIEW_H
#define LOADING_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "game_resources.h"
#include "resource_loader.h"

/// Loading screen
class loading_view
{
public:
  loading_view();

  /// Run the loading, until the user quits
  void exec();

  auto& get_resources() noexcept { return m_resources; }

  auto& get_window() noexcept { return m_window; }

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  /// The window to draw to
  sf::RenderWindow m_window;

  resource_loader m_resource_loader;

  /// Resources
  game_resources m_resources;

  /// Run the menu
  void exec_menu();

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the loading on-screen
  void show();
};

#endif // LOGIC_ONLY

#endif // LOADING_VIEW_H
