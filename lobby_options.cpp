#include "lobby_options.h"

#include <cassert>
#include <iostream>
#include <sstream>

lobby_options::lobby_options(
  const chess_color lhs_color,
  const race lhs_race,
  const race rhs_race
)
  : m_lhs_color{lhs_color},
    m_lhs_race{lhs_race},
    m_rhs_race{rhs_race}
{

}

lobby_options create_default_lobby_options() noexcept
{
  return lobby_options(chess_color::white, race::classic, race::classic);
}

chess_color lobby_options::get_color(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_color;
  }
  assert(player_side == side::rhs);
  return get_other_color(m_lhs_color);
}

chess_color get_color(const lobby_options& options, const side player_side) noexcept
{
  return options.get_color(player_side);
}

race lobby_options::get_race(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_race;
  }
  assert(player_side == side::rhs);
  return m_rhs_race;
}

race get_race_of_color(const lobby_options& options, const chess_color c) noexcept
{
  if (options.get_color(side::lhs) == c) return options.get_race(side::lhs);
  assert(options.get_color(side::rhs) == c);
  return options.get_race(side::rhs);
}

void lobby_options::set_color(const chess_color color, const side player_side) noexcept
{
  if (player_side == side::lhs)
  {
    m_lhs_color = color;
  }
  else
  {
    assert(player_side == side::rhs);
    m_lhs_color = get_other_color(color);
  }
}

void lobby_options::set_race(const race r, const side player_side) noexcept
{
  if (player_side == side::lhs)
  {
    m_lhs_race = r;
  }
  else
  {
    assert(player_side == side::rhs);
    m_rhs_race = r;
  }
}

void test_lobby_options()
{
  #ifndef NDEBUG
  // get_color and set_color
  {
    lobby_options options{create_default_lobby_options()};
    options.set_color(chess_color::white, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::white);
    options.set_color(chess_color::black, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::black);
    options.set_color(chess_color::white, side::rhs);
    assert(options.get_color(side::rhs) == chess_color::white);
    options.set_color(chess_color::black, side::rhs);
    assert(options.get_color(side::rhs) == chess_color::black);
  }
  // get_race and set_race
  {
    lobby_options options{create_default_lobby_options()};
    options.set_race(race::protoss, side::lhs);
    assert(options.get_race(side::lhs) == race::protoss);
    options.set_race(race::zerg, side::lhs);
    assert(options.get_race(side::lhs) == race::zerg);
    options.set_race(race::protoss, side::rhs);
    assert(options.get_race(side::rhs) == race::protoss);
    options.set_race(race::zerg, side::rhs);
    assert(options.get_race(side::rhs) == race::zerg);
  }
  // 76: set_color ensures the other player has the other color
  {
    lobby_options options{create_default_lobby_options()};
    options.set_color(chess_color::white, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::white);
    assert(options.get_color(side::rhs) == chess_color::black);
    options.set_color(chess_color::black, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::black);
    assert(options.get_color(side::rhs) == chess_color::white);
    options.set_color(chess_color::white, side::rhs);
    assert(options.get_color(side::lhs) == chess_color::black);
    assert(options.get_color(side::rhs) == chess_color::white);
    options.set_color(chess_color::black, side::rhs);
    assert(options.get_color(side::lhs) == chess_color::white);
    assert(options.get_color(side::rhs) == chess_color::black);
  }
  // ::get_color
  {
    const lobby_options options{create_default_lobby_options()};
    assert(get_color(options, side::lhs) == chess_color::white);
    assert(get_color(options, side::rhs) == chess_color::black);
  }
  // get_race_of_color
  {
    const lobby_options options{create_default_lobby_options()};
    assert(get_color(options, side::lhs) == chess_color::white);
    assert(get_color(options, side::rhs) == chess_color::black);
    assert(get_race_of_color(options, chess_color::white) == options.get_race(side::lhs));
    assert(get_race_of_color(options, chess_color::black) == options.get_race(side::rhs));
  }
  // operator<<
  {
    const lobby_options options{create_default_lobby_options()};
    std::stringstream s;
    s << options;
    assert(!s.str().empty());
  }
  #endif
}

std::ostream& operator<<(std::ostream& os, const lobby_options& options) noexcept
{
  os
    << "LHS color: " << options.get_color(side::lhs) << '\n'
    << "RHS color: " << options.get_color(side::rhs) << '\n'
    << "LHS race: " << options.get_race(side::lhs) << '\n'
    << "RHS race: " << options.get_race(side::rhs)
  ;
  return os;
}
