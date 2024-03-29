#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "ccfwd.h"
#include "game_options.h"
#include "game_resources.h"
#include "menu_view_layout.h"
#include "menu_view_item.h"
#include "physical_controllers.h"

class menu_view
{
public:
  menu_view(
    const game_options& options,
    const physical_controllers& controllers
  );

  /// Run the menu, until the user quits
  void exec();

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_resources; }

  const auto& get_selected() const noexcept { return m_selected; }

  auto& get_window() noexcept { return m_window; }

  /// Sets the selected item.
  /// If the selected item changes, play sound
  void set_selected(const menu_view_item i);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  /// The layout of this window
  menu_view_layout m_layout;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// The game options
  game_options m_game_options;

  /// The current physical controllers
  physical_controllers m_physical_controllers;

  /// Resources
  game_resources m_resources;

  /// The selected item
  menu_view_item m_selected;

  /// Run the about screen
  void exec_about();

  /// Run the game
  void exec_game();

  /// Run the lobby
  void exec_lobby();

  /// Run the options screen
  void exec_options();

  /// View the played game
  void exec_played_game();

  /// Run the game with a replay
  void exec_replay();

  /// Run the dialog under 'Start'
  void exec_start();

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

void show_about_panel(menu_view& v);
void show_image_panel(menu_view& v);
void show_options_panel(menu_view& v);

/// Show where the panels will be drawn
void show_layout_panels(menu_view& v);

void show_quit_panel(menu_view& v);
void show_selected_panel(menu_view& v);
void show_subtitle_panel(menu_view& v);
void show_start_panel(menu_view& v);
void show_title_panel(menu_view& v);


#endif // LOGIC_ONLY

#endif // MENU_VIEW_H
