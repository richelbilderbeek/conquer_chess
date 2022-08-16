#ifndef PLAYED_GAME_VIEW_H
#define PLAYED_GAME_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "game_resources.h"
#include "played_game_view_layout.h"
#include "game.h"

/// View a played Conquer Chess game
class played_game_view
{
public:
  played_game_view(const game& g);

  /// Run the menu, until the user quits
  void exec();

  const auto& get_game() const noexcept { return m_game; }

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_resources; }

  auto& get_window() noexcept { return m_window; }

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  /// The layout of this window
  played_game_view_layout m_layout;

  /// The played game
  game m_game;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Resources
  game_resources m_resources;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

/// Show where the panels will be drawn
void show_panels(played_game_view& v);

/// Show where the panels will be drawn
void show_text_panel(played_game_view& v);

#endif // LOGIC_ONLY

#endif // PLAYED_GAME_VIEW_H
