#ifndef SQUARE_H
#define SQUARE_H

#include <iosfwd>
#include <random>
#include <string>
#include <vector>
#include "ccfwd.h"
#include "chess_color.h"
#include "castling_type.h"

/// A chess square, e.g. e4
class square
{
public:
  explicit square(const std::string& pos);
  explicit square(const game_coordinat& g);
  explicit square(const int x, const int y);

  /// Get the x coordinat, starting from 0 for a1/b1/c1/etc.
  /// As the board goes from a1 at top-left,
  /// to a8 at top-right,
  /// the x coordinat is the rank
  int get_x() const noexcept { return m_x; }

  /// Get the y coordinat, starting from 0 for a1/a2/a3/etc.
  /// As the board goes from a1 at top-left,
  /// to a8 at top-right,
  /// the y coordinat is the file
  int get_y() const noexcept { return m_y; }

private:

  int m_x;
  int m_y;
};

/// Are the squares adjacent (i.e. for a king, not a knight)
/// @see are_adjacent_for_knight to determine if squares are adjacent
///   for a knight
bool are_adjacent(const square& a, const square& b) noexcept;

/// Are the squares adjacent for a knight
/// @see are_adjacent to determine if squares are adjacent
///   for a king
bool are_adjacent_for_knight(const square& a, const square& b) noexcept;

/// Are all the squares unique?
bool are_all_unique(std::vector<square> squares);

/// Are the squares adjacent on the same diagonal, e.g. d1 and e2
bool are_on_adjacent_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same diagonal, e.g. d1 and a4
bool are_on_same_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same file, e.g. e2 and e4
bool are_on_same_file(const square& a, const square& b) noexcept;

/// Are the squares on the same half-diagonal,
/// also 'the jump of a knight'
/// e.g. b1 and c3
bool are_on_same_half_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same rank, e.g. a1 and a8
bool are_on_same_rank(const square& a, const square& b) noexcept;

/// Collect all the delta squares for a bishop
std::vector<std::pair<int, int>> collect_all_bishop_delta_pairs() noexcept;

/// Collect all the possible target squares for a bishop
/// in all directions
std::vector<std::vector<square>> collect_all_bishop_target_squares(const square& s) noexcept;

/// Collect all the possible target squares for a king
std::vector<square> collect_all_king_target_squares(const square& s) noexcept;

/// Collect all the delta squares for a knight
std::vector<std::pair<int, int>> collect_all_knight_delta_pairs() noexcept;

/// Collect all the possible target squares for a knight
std::vector<square> collect_all_knight_target_squares(const square& s) noexcept;

/// Collect all the delta squares for a queen
std::vector<std::pair<int, int>> collect_all_queen_delta_pairs() noexcept;

/// Collect all the possible target squares for a queen
/// in all directions
std::vector<std::vector<square>> collect_all_queen_target_squares(const square& s) noexcept;

/// Collect all the delta squares for a rook
std::vector<std::pair<int, int>> collect_all_rook_delta_pairs() noexcept;

/// Collect all the possible target squares for a rook
/// in all directions
std::vector<std::vector<square>> collect_all_rook_target_squares(const square& s) noexcept;

/// Concatenate the vectors
std::vector<square> concatenate(
  const std::vector<square>& a,
  const std::vector<square>& b
);

/// Create a random square
square create_random_square(
  std::default_random_engine& rng_engine
);

/// Get the square behind a pawn.
/// Examples:
///  * for white, behind e4 would be e3.
///  * for black, behind e5 would be e6.
square get_behind(
  const square& pawn_square,
  const chess_color color
);

/// Get the square besides a pawn.
/// Examples:
///  * 'e4' would have 'd4' and 'f4'
///  * 'h7' would have 'g7'
std::vector<square> get_besides(
  const square& pawn_square
);

/// Get the default king square.
/// These are:
///  * e1 for white
///  * e8 for black
square get_default_king_square(const chess_color player_color) noexcept;

/// Get the default rook square,
/// for a castling direction.
/// These are:
///  * h1 for white for a kingside castle
///  * a1 for white for a queenside castle
///  * h8 for black for a kingside castle
///  * a8 for black for a queenside castle
square get_default_rook_square(
  const chess_color player_color,
  const castling_type t
) noexcept;

/// Get the file of a square, e.g. 'd' from 'd4'
char get_file(const square& s) noexcept;

/// Get the file of a square, e.g. 'd' from 'd4'
char get_file_char(const square& s) noexcept;

/// Get the intermediate squares, in an inclusive way:
/// the first square will be 'from',
/// to last square will be 'to'
std::vector<square> get_intermediate_squares(
  const square& from,
  const square& to
);

/// Get the rank of a square, e.g. '3' from 'e3'
int get_rank(const square& s) noexcept;

/// Is the square 's' occupied?
bool is_occupied(
  const square& s,
  const std::vector<square>& occupied_squares
) noexcept;

/// Can the x and y be used to create a valid square?
bool is_valid_square_xy(const int x, const int y) noexcept;

/// Can the x and y not be used to create a valid square?
bool is_invalid_square_xy(const int x, const int y) noexcept;

/// Test this class and its free functions
void test_square();

/// What is the color of this square?
chess_color to_color(const square& s) noexcept;

// a1 == (0.5, 0.5)
// b1 == (0.5, 1.5)
game_coordinat to_coordinat(const square& s) noexcept;

// a1 == (0.5, 0.5)
// b1 == (0.5, 1.5)
game_coordinat to_coordinat(const std::string& notation) noexcept;


/// Convert a square to a rectangle
/// For example, a1 == ((0,0)-(1,1)) (notation is top-left, then bottom-left)
/// For example, b1 == ((0,1)-(1,2)) (notation is top-left, then bottom-left)
game_rect to_game_rect(const square& s) noexcept;

/// Convert the pairs of x-y-coordinats to valid squares.
std::vector<square> to_squares(std::vector<std::pair<int, int>> xys);

std::string to_str(const square& s) noexcept;

std::string to_str(const std::vector<square>& s) noexcept;

bool operator==(const square& lhs, const square& rhs) noexcept;
bool operator!=(const square& lhs, const square& rhs) noexcept;
bool operator<(const square& lhs, const square& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const square& s) noexcept;
std::ostream& operator<<(std::ostream& os, const std::vector<square>& s) noexcept;

#endif // SQUARE_H
