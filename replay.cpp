#include "replay.h"

#include "helper.h"
#include "game.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

replay::replay(const std::string& pgn_str)
{
  if (pgn_str.empty()) return;

  assert(!pgn_str.empty());
  assert(std::count(std::begin(pgn_str), std::end(pgn_str), '\n') == 0);
  const std::vector<std::string> pgn_moves{
    split_pgn_str(pgn_str)
  };

  m_moves.reserve(pgn_moves.size());
  chess_color color{chess_color::white};
  for (const auto& pgn_move: pgn_moves)
  {
    m_moves.push_back(chess_move(pgn_move, color));
    color = get_other_color(color);
  }
}

int get_n_moves(const replay& r) noexcept
{
  return static_cast<int>(r.get_moves().size());
}


std::string get_replay_1_as_pgn_str() noexcept
{
  return "1. e4 e6 2. d4 b6 3. a3 Bb7 4. Nc3 Nh6 5. Bxh6 gxh6 6. Be2 Qg5 7. Bg4 h5 8. Nf3 Qg6 9. Nh4 Qg5 10. Bxh5 Qxh4 11. Qf3 Kd8 12. Qxf7 Nc6 13. Qe8# 1-0";
}

std::string get_scholars_mate_as_pgn_str() noexcept
{
  return "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0";
}

void play_game(const replay& r, const bool verbose)
{
  game g;
  const auto& moves{r.get_moves()};
  const int n_moves = moves.size();
  for (int i{0}; i!=n_moves; ++i)
  {
    const auto& m{moves[i]};
    if (verbose) std::clog << i << ": " << m << '\n';
    start_chess_move(g, m);
    for (int j{0}; j!=4; ++j) g.tick(delta_t(0.25));
    if (verbose) std::clog << to_board_str(
      g.get_pieces(),
      board_to_text_options(true, true)
    ) << '\n';
    const auto messages{collect_messages(g)};
    const long n_cannot{
      std::count_if(
        std::begin(messages),
        std::end(messages),
        [](const auto& msg)
        {
          return msg.get_message_type() == message_type::cannot;
        }
      )
    };
    assert(n_cannot == 0);
  }
}

std::vector<std::string> split_pgn_str(const std::string pgn_str)
{
  std::vector<std::string> strings{
    split_str(pgn_str, ' ')
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

void test_replay()
{
#ifndef NDEBUG
  // split_pgn_str
  {
    assert(split_pgn_str("1. e4").size() == 1);
  }
  // replay::replay from empty string is OK
  {
    const replay r("");
    assert(get_n_moves(r) == 0);
  }
  // replay::replay from get_scholars_mate_as_pgn_str
  {
    const replay r(get_scholars_mate_as_pgn_str());
    assert(get_n_moves(r) == 8);
  }
  // replay::replay from get_replay_1_as_pgn_str
  {
    const replay r(get_replay_1_as_pgn_str());
    assert(get_n_moves(r) > 8);
  }
  //#define FIX_ISSUE_22
  #ifdef FIX_ISSUE_22
  // play replay of scholars mate
  {
    const replay r(get_scholars_mate_as_pgn_str());
    play_game(r);
  }
  // play replay 1
  {
    const replay r(get_replay_1_as_pgn_str());
    play_game(r);
  }
  #endif
  // 45: operator<<
  {
    const replay r(get_replay_1_as_pgn_str());
    std::stringstream s;
    s << r;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const replay& lhs, const replay& rhs) noexcept
{
  return lhs.get_moves() == rhs.get_moves();
}

std::ostream& operator<<(std::ostream& os, const replay& r) noexcept
{
  std::stringstream s;
  const int n = r.get_moves().size();
  for (int i{0}; i!=n; ++i)
  {
    s << i << ": " << r.get_moves()[i] << ", ";
  }
  std::string t{s.str()};
  if (t.empty()) return os;
  assert(!t.empty());
  t.pop_back();
  assert(!t.empty());
  t.pop_back();
  os << t;
  return os;
}
