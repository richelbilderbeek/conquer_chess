#ifndef REPLAY_H
#define REPLAY_H

#include "chess_move.h"

#include <string>
#include <vector>

/// A collection of chess moves
class replay
{
public:
  /// @param pgn_str a string of PGN notation in one line,
  /// e.g.
  replay(const std::string& pgn_str);

  const auto& get_moves() const noexcept { return m_moves; }

private:

  std::vector<chess_move> m_moves;

};

std::string get_replay_str_1() noexcept;

std::string get_scholars_mate_png_str() noexcept;

replay get_replay_1() noexcept;

/// Test this class and its free functions
void test_replay();


#endif // REPLAY_H
