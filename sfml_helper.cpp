#include "sfml_helper.h"

#ifndef LOGIC_ONLY

#include <cassert>
#include <cmath>

sf::Color f_health_to_color(const double f)
{
  assert(f >= 0.0);
  assert(f <= 1.0);
  if (f < 0.25) return sf::Color::Red;
  if (f < 0.50) return sf::Color(255, 128, 0);
  if (f < 0.75) return sf::Color::Yellow;
  return sf::Color::Green;
  /*
  sf::Color(
    static_cast<sf::Uint8>(f * 255.0),
    static_cast<sf::Uint8>(f * 255.0),
    static_cast<sf::Uint8>(f * 255.0)
  )
  */
}

void set_rect(sf::RectangleShape& rectangle, const screen_rect& screen_rect)
{
  rectangle.setOrigin(
    get_width(screen_rect) / 2,
    get_height(screen_rect) / 2
  );
  rectangle.setPosition(
    get_center(screen_rect).get_x(),
    get_center(screen_rect).get_y()
  );
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
}

void set_text_position(sf::Text& text, const screen_rect& screen_rect)
{
  assert(text.getString().getSize() > 0);
  // https://en.sfml-dev.org/forums/index.php?topic=15875.msg113439#msg113439
  text.setPosition(
    get_center(screen_rect).get_x(),
    get_center(screen_rect).get_y()
  );
  // https://en.sfml-dev.org/forums/index.php?topic=15875.msg113439#msg113439
  text.setOrigin(
    std::round(text.getLocalBounds().width / 2.0),
    std::round(text.getLocalBounds().height / 2.0)
  );
}

sf::Color to_sfml_color(const chess_color color) noexcept
{
  if (color == chess_color::white) return sf::Color::White;
  assert(color == chess_color::black);
  return sf::Color::Black;
}

#endif // LOGIC_ONLY
