#include "game_options.h"

#include "game_view_layout.h"
#include <cassert>

game_options::game_options(
  const screen_coordinat& screen_size,
  const chess_color player_color,
  const std::vector<piece>& starting_pieces,
  const double delta_t,
  const int margin_width
) : m_delta_t{delta_t},
    m_margin_width{margin_width},
    m_player_color{player_color},
    m_screen_size{screen_size},
    m_starting_pieces{starting_pieces}
{
  assert(m_delta_t > 0.0);
  assert(m_margin_width >= 0);
  assert(m_screen_size.get_x() > 0);
  assert(m_screen_size.get_y() > 0);
  assert(m_starting_pieces.size() >= 2);
}

game_options get_default_game_options()
{
  return game_options(
    get_default_screen_size(),
    chess_color::white,
    get_default_starting_pieces(),
    get_default_delta_t(),
    get_default_margin_width()
  );
}

double get_default_delta_t()
{
  return 0.00001;
}

void game_options::set_player_color(const chess_color c) noexcept
{
  m_player_color = c;
}

void toggle_player(game_options& options)
{
  options.set_player_color(
    get_other_color(options.get_player_color())
  );
}
