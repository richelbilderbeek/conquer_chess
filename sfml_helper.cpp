#include "sfml_helper.h"

#ifndef LOGIC_ONLY

#include <cassert>
#include <cmath>

void set_rect(sf::RectangleShape& rectangle, const screen_rect& screen_rect)
{
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
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

#endif // LOGIC_ONLY
