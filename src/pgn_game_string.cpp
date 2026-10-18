#include "pgn_game_string.h"

#include "helper.h"

#include <cassert>
#include <iterator>


pgn_game_string::pgn_game_string(const std::string& pgn_str)
  : m_pgn_str{pgn_str}
{

}

pgn_game_string get_kasparov_vs_topalov_as_pgn_str() noexcept
{
  return pgn_game_string("1.e4 d6 2.d4 Nf6 3.Nc3 g6 4.Be3 Bg7 5.Qd2 c6 6.f3 b5 7.Nge2 Nbd7 8.Bh6 Bxh6 9.Qxh6 Bb7 10.a3 e5 11.0-0-0 Qe7 12.Kb1 a6 13.Nc1 0-0-0 14.Nb3 exd4 15.Rxd4 c5 16.Rd1 Nb6 17.g3 Kb8 18.Na5 Ba8 19.Bh3 d5 20.Qf4+ Ka7 21.Rhe1 d4 22.Nd5 Nbxd5 23.exd5 Qd6 24.Rxd4!! cxd4 25.Re7+ Kb6 26.Qxd4+ Kxa5 27.b4+ Ka4 28.Qc3 Qxd5 29.Ra7 Bb7 30.Rxb7 Qc4 31.Qxf6 Kxa3 32.Qxa6+ Kxb4 33.c3+ Kxc3 34.Qa1+ Kd2 35.Qb2+ Kd1 36.Bf1! Rd2 37.Rd7!! Rxd7 38.Bxc4 bxc4 39.Qxh8 Rd3 40.Qa8 c3 41.Qa4+ Ke1 42.f4 f5 43.Kc1 Rd2 44.Qa7 1-0");
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
    const pgn_game_string s{get_kasparov_vs_topalov_as_pgn_str()};
    assert(!s.get().empty());
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
