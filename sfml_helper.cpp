#include "sfml_helper.h"

#ifndef LOGIC_ONLY

#include <cassert>
#include <cmath>

void set_rect(sf::RectangleShape& rectangle, const screen_rect& screen_rect)
{
  if (1 + 1 == 2)
  {
    rectangle.setOrigin(
      get_width(screen_rect) / 2,
      get_height(screen_rect) / 2
    );
    rectangle.setPosition(
      get_center(screen_rect).get_x(),
      get_center(screen_rect).get_y()
    );
  }
  else
  {
    rectangle.setOrigin(
      0.0, 0.0
    );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
  );
  }
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
