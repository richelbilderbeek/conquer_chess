#include "starting_position_type.h"

#include <cassert>

starting_position_type get_next(const starting_position_type starting_position) noexcept
{
  switch (starting_position)
  {
    case starting_position_type::standard:
      return starting_position_type::kings_only;
    case starting_position_type::kings_only:
      return starting_position_type::before_scholars_mate;
    case starting_position_type::before_scholars_mate:
      return starting_position_type::bishop_and_knight_end_game;
    default:
    case starting_position_type::bishop_and_knight_end_game:
      assert(starting_position == starting_position_type::bishop_and_knight_end_game);
      return starting_position_type::standard;
  }
}


void test_starting_position_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(starting_position_type::standard) == "standard");
    assert(to_str(starting_position_type::kings_only) == "kings_only");
    assert(to_str(starting_position_type::before_scholars_mate) == "before_scholars_mate");
    assert(to_str(starting_position_type::bishop_and_knight_end_game) == "bishop_and_knight_end_game");
  }
  // get_next
  {
    assert(get_next(starting_position_type::standard) == starting_position_type::kings_only);
    assert(get_next(starting_position_type::kings_only) == starting_position_type::before_scholars_mate);
    assert(get_next(starting_position_type::before_scholars_mate) == starting_position_type::bishop_and_knight_end_game);
    assert(get_next(starting_position_type::bishop_and_knight_end_game) == starting_position_type::standard);
  }
#endif // DEBUG
}

std::string to_str(const starting_position_type t) noexcept
{
  switch(t)
  {
    case starting_position_type::standard:
      return "standard";
    case starting_position_type::kings_only:
      return "kings_only";
    case starting_position_type::before_scholars_mate:
      return "before_scholars_mate";
    default:
    case starting_position_type::bishop_and_knight_end_game:
      assert(t == starting_position_type::bishop_and_knight_end_game);
      return "bishop_and_knight_end_game";
  }
}
