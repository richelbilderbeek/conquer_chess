#ifndef REPLAY_H
#define REPLAY_H

#include "chess_move.h"

#include <iosfwd>
#include <string>
#include <vector>

/// A collection of chess moves
class replay
{
public:
  /// @param pgn_str a string of PGN notation in one line,
  /// e.g.
  explicit replay(const std::string& pgn_str);

  const auto& get_moves() const noexcept { return m_moves; }

private:

  std::vector<chess_move> m_moves;

};

/// Get the number of moves in the replay
int get_n_moves(const replay& r) noexcept;

std::string get_replay_1_as_pgn_str() noexcept;

std::string get_scholars_mate_as_pgn_str() noexcept;

replay get_replay_1() noexcept;

/// Play a replay, using \link{game} (not \link{game_controller})
/// as the actor
void play_game(const replay& r);

/// Split the PGN string into its moves
/// E.g. '1. e4 e5 2. Nc3' will be split into {'e4', 'e5', 'Nc3'}
std::vector<std::string> split_pgn_str(const std::string pgn_str);

/// Test this class and its free functions
void test_replay();

bool operator==(const replay& lhs, const replay& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const replay& r) noexcept;

#endif // REPLAY_H
