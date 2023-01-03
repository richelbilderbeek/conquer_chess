#ifndef LOBBY_VIEW_H
#define LOBBY_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "game_options.h"
#include "game_resources.h"
#include "lobby_view_layout.h"
#include "lobby_view_item.h"

class lobby_view
{
public:
  lobby_view();

  /// Run the menu, until the user quits
  void exec();

  /// Get the chess color of a player
  chess_color get_color(const side player_side) const noexcept;

  /// Get the chess color of a player
  race get_race(const side player_side) const noexcept;

  /// Get the selected item of a player
  lobby_view_item get_selected(const side player_side) const noexcept;

  /// Is the player ready?
  bool get_start(const side player_side) const noexcept;

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_resources; }


  auto& get_window() noexcept { return m_window; }

  /// Sets the selected item.
  /// If the selected item changes, play sound
  void set_selected(const lobby_view_item i);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  /// The layout of this window
  lobby_view_layout m_layout;

  /// The physical controllers
  physical_controllers m_physical_controllers;

  /// Resources
  game_resources m_resources;

  /// The selected color
  chess_color m_lhs_color;
  chess_color m_rhs_color;

  /// The selected color
  race m_lhs_race;
  race m_rhs_race;

  /// The selected item
  lobby_view_item m_lhs_selected;
  lobby_view_item m_rhs_selected;

  /// Ready to start?
  bool m_lhs_start;
  bool m_rhs_start;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Run the about screen
  void exec_about();

  /// Run the game
  void exec_game();

  /// Run the options screen
  void exec_options();

  /// View the played game
  void exec_played_game();

  /// Run the game with a replay
  void exec_replay();

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

/// The pretty image at the top
void show_image_panel(lobby_view& v);

/// Show where the panels will be drawn
void show_panels(lobby_view& v);

/// Draw the cursor on the selected panel
void show_selected_panel(lobby_view& v, const side player_side);

void show_color_panel(lobby_view& v, const side player_side);
void show_race_panel(lobby_view& v, const side player_side);
void show_start_panel(lobby_view& v, const side player_side);


#endif // LOGIC_ONLY

#endif // LOBBY_VIEW_H
