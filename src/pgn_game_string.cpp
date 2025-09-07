#include "pgn_game_string.h"

#include "helper.h"

#include <cassert>
#include <iterator>


pgn_game_string::pgn_game_string(const std::string& pgn_str)
  : m_pgn_str{pgn_str}
{

}

pgn_game_string get_replay_1_as_pgn_str() noexcept
{
  return pgn_game_string("1. e4 e6 2. d4 b6 3. a3 Bb7 4. Nc3 Nh6 5. Bxh6 gxh6 6. Be2 Qg5 7. Bg4 h5 8. Nf3 Qg6 9. Nh4 Qg5 10. Bxh5 Qxh4 11. Qf3 Kd8 12. Qxf7 Nc6 13. Qe8# 1-0");
}

pgn_game_string get_scholars_mate_as_pgn_str() noexcept
{
  return pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0");
}

std::vector<std::string> split_pgn_str(const pgn_game_string& pgn_str)
{
  std::vector<std::string> strings{
    split_str(pgn_str.get(), ' ')
  };
  std::vector<std::string> move_strings;
  move_strings.reserve(1 + (2 * strings.size() / 3));
  std::copy_if(
    std::begin(strings),
    std::end(strings),
    std::back_inserter(move_strings),
    [](const std::string& s)
    {
      assert(!s.empty());
      return s.back() != '.';
    }
  );
  return move_strings;
}

void test_pgn_game_string()
{
  #ifndef NDEBUG
  {
    const pgn_game_string s;
  }
  {
    const std::string s{"1. e4"};
    const pgn_game_string t(s);
    assert(t.get() == s);
  }
  // get_replay_1_as_pgn_str
  {
    const pgn_game_string s{get_replay_1_as_pgn_str()};
    assert(!s.get().empty());
  }
  // get_scholars_mate_as_pgn_str
  {
    const pgn_game_string s{get_scholars_mate_as_pgn_str()};
    assert(!s.get().empty());
  }
  // split_pgn_str
  {
    assert(split_pgn_str(pgn_game_string("1. e4")).size() == 1);
    assert(split_pgn_str(pgn_game_string("1. e4 e5")).size() == 2);
  }
  // operator==
  {
    const auto a{get_scholars_mate_as_pgn_str()};
    const auto b{get_scholars_mate_as_pgn_str()};
    const auto c{get_replay_1_as_pgn_str()};
    assert(a == b);
    assert(!(a == c));

  }
  #endif
}

bool operator==(const pgn_game_string& lhs, const pgn_game_string& rhs) noexcept
{
  return lhs.get() == rhs.get();
}
