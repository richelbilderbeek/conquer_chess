#ifndef ABOUT_VIEW_H
#define ABOUT_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "game_resources.h"
#include "about_view_layout.h"
#include "about_view_item.h"

class about_view
{
public:
  about_view();

  /// Run the menu, until the user quits
  void exec();

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_resources; }

  /// The rotation, in degrees
  double get_rotation() const noexcept { return m_rotation; };

  auto& get_window() noexcept { return m_window; }

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  /// The game clock, to measure the elapsed time
  sf::Clock m_clock;

  /// The layout of this window
  about_view_layout m_layout;

  double m_rotation;

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
void show_layout_panels(about_view& v);

void show_subtitle_panel(about_view& v);
void show_text_panel(about_view& v);
void show_title_panel(about_view& v);


#endif // LOGIC_ONLY

#endif // ABOUT_VIEW_H
