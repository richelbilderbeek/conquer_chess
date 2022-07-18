#ifndef OPTIONS_VIEW_H
#define OPTIONS_VIEW_H

#ifndef LOGIC_ONLY

#include "options_view_layout.h"

#include <SFML/Graphics.hpp>

class options_view
{
public:
  options_view();

  /// Run the menu, until the user quits
  void exec();

  const auto& get_layout() const noexcept { return m_layout; }

  const auto& get_window() const noexcept { return m_window; }

  auto& get_window() noexcept { return m_window; }

private:

  options_view_layout m_layout;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

/// Show all the layout panels
void show_panels(options_view& v);

#endif // LOGIC_ONLY

#endif // OPTIONS_VIEW_H
