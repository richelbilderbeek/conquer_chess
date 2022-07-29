#ifndef OPTIONS_VIEW_H
#define OPTIONS_VIEW_H

#ifndef LOGIC_ONLY

#include "game_options.h"
#include "options_view_layout.h"
#include "options_view_item.h"
#include "game_resources.h"

#include <SFML/Graphics.hpp>

class options_view
{
public:
  explicit options_view(const game_options& options);

  /// Run the menu, until the user quits
  void exec();

  const auto& get_layout() const noexcept { return m_layout; }

  const auto& get_options() const noexcept { return m_options; }

  auto& get_resources() noexcept { return m_resources; }

  options_view_item get_selected() const noexcept { return m_selected; }

  const auto& get_window() const noexcept { return m_window; }

  auto& get_window() noexcept { return m_window; }

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  options_view_layout m_layout;

  /// The game options
  game_options m_options;

  game_resources m_resources;

  /// The selected item
  options_view_item m_selected;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

/// Get the starting position
starting_position_type get_starting_position(const options_view& v) noexcept;

/// Show the bottom part
void show_bottom(options_view& v);

/// Show the header of the bottom part
void show_bottom_header(options_view& v);

/// Show the left player's settings of the bottom part
void show_bottom_left(options_view& v);

/// Show the right player's settings of the bottom part
void show_bottom_right(options_view& v);

/// Show the game speed label and value
void show_game_speed(options_view& v);

/// Show the game speed label and value
void show_music_volume(options_view& v);

/// Show all the layout panels
void show_panels(options_view& v);

void show_starting_position(options_view& v);

/// Show the selected panel
void show_selected_panel(options_view& v);

/// Show the top part
void show_top(options_view& v);

#endif // LOGIC_ONLY

#endif // OPTIONS_VIEW_H
