#ifndef PGN_GAME_STRING_H
#define PGN_GAME_STRING_H

#include <string>
#include <vector>

/// A string that is a PGN notation string of a game.
///
/// For example, a game with a scholar's mate can have the PGN string
/// `1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0`
///
/// @seealso use \link{pgn_move_string} for the string of a move, e.g. `e4`
class pgn_game_string
{
public:
  pgn_game_string(const std::string& pgn_str = "");

  const auto& get() const noexcept { return m_pgn_str; }

private:
  std::string m_pgn_str;
};

/// Number 1 at
/// https://www.raindropchess.com/the-greatest-chess-games-of-all-time-a-curated-ranking/
/// Kasparov vs Topalov, Wijk aan Zee 1999
pgn_game_string get_kasparov_vs_topalov_as_pgn_str() noexcept;

pgn_game_string get_replay_1_as_pgn_str() noexcept;



/// Get the PGN for a scholar's mate.
///
/// The PGN string is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
///
/// The positions of this scholar's mate
/// are created by \link{fen_string get_fen_string_scholars_mate_0},
/// \link{fen_string get_fen_string_scholars_mate_1}, etc.
pgn_game_string get_scholars_mate_as_pgn_str() noexcept;

/// Split the PGN string into its moves
/// E.g. '1. e4 e5 2. Nc3' will be split into {'e4', 'e5', 'Nc3'}
std::vector<std::string> split_pgn_str(const pgn_game_string& pgn_str);

void test_pgn_game_string();

bool operator==(const pgn_game_string& lhs, const pgn_game_string& rhs) noexcept;


#endif // PGN_GAME_STRING_H
