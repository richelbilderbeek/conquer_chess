#include "lobby_options.h"

#include <cassert>

lobby_options::lobby_options()
  : m_lhs_color{chess_color::white},
    m_lhs_race{race::terran},
    m_rhs_race{race::terran}
{

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
    lobby_options options;
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
    lobby_options options;
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
    lobby_options options;
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
    const lobby_options options;
    assert(get_color(options, side::lhs) == chess_color::white);
    assert(get_color(options, side::rhs) == chess_color::black);
  }
  #endif
}
