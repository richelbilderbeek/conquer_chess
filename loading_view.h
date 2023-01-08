#ifndef LOADING_VIEW_H
#define LOADING_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "game_resources.h"
#include "resource_loader.h"
#include "game_options.h"
#include "physical_controllers.h"

/// Loading screen
class loading_view
{
public:
  loading_view(
    const game_options& go,
    const physical_controllers& cs
  );

  /// Run the loading, until the user quits
  void exec();

  auto& get_resources() noexcept { return m_resources; }

  auto& get_window() noexcept { return m_window; }

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  /// The initial game options, as can be set in the command-line
  game_options m_game_options;

  /// The initial physical controllers, as can be set in the command-line
  physical_controllers m_physical_controllers;

  /// For loading the resources in steps
  resource_loader m_resource_loader;

  /// Resources
  game_resources m_resources;

  /// The window to draw to
  sf::RenderWindow m_window;

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
