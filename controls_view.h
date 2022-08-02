#ifndef CONTROLS_VIEW_H
#define CONTROLS_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "controller.h"
#include "game_resources.h"
#include "controls_view_layout.h"
#include "controls_view_item.h"

class controls_view
{
public:
  controls_view(controller c = controller(controller_type::keyboard, side::lhs));

  /// Run the menu, until the user quits
  void exec();

  /// Get the controller, as changed by this view
  const auto& get_controller() const noexcept { return m_controller; }

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_resources; }

  const auto& get_selected() const noexcept { return m_selected; }

  auto& get_window() noexcept { return m_window; }

private:

  /// The layout of this window
  controls_view_layout m_layout;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// The controller
  controller m_controller;

  /// Resources
  game_resources m_resources;

  /// The selected item
  controls_view_item m_selected;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

/// Show where the panels will be drawn
void show_panels(controls_view& v);

#endif // LOGIC_ONLY

#endif // CONTROLS_VIEW_H
