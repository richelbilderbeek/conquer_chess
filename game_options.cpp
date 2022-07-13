#include "game_options.h"

#include <cassert>

game_options::game_options(
  const screen_coordinat& screen_size,
  const double delta_t,
  const int margin_width
) : m_delta_t{delta_t},
    m_margin_width{margin_width},
    m_screen_size{screen_size}
{
  assert(m_delta_t > 0.0);
  assert(m_margin_width >= 0);
  assert(m_screen_size.get_x() > 0);
  assert(m_screen_size.get_y() > 0);
}

game_options get_default_game_options()
{
  return game_options(
    get_default_screen_size(), // screen_size
    0.00001, // delta_t
    10 // margin_width
  );
}
