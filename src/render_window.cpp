#include "render_window.h"

#ifndef LOGIC_ONLY

#include "screen_rect.h"

sf::RenderWindow& get_render_window() noexcept {

  // const auto modes = sf::VideoMode::getFullscreenModes();


  static sf::RenderWindow window{
    sf::VideoMode(
      get_width(get_default_screen_rect()),
      get_height(get_default_screen_rect())
    ),
    "Conquer Chess",
    sf::Style::Fullscreen
  };

  return window;
}

#endif // LOGIC_ONLY
