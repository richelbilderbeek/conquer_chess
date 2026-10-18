#ifndef PIECES_H
#define PIECES_H

/// @file Functions to work on collections of pieces

#include "ccfwd.h"
#include "board_to_text_options.h"
#include "piece.h"
#include "starting_position_type.h"

#include <optional>

/// Surround the 8x8 strings of a chessboard by coordinats
/// Without coordinats:
///
/// rp....PR
/// np....PN
/// bp....PB
/// qp....PQ
/// kp....PK
/// bp....PB
/// np....PN
/// rp....PR
///
/// With coordinats:
///
///   12345678
///  +--------+
/// A|rp....PR|A
/// B|np....PN|B
/// C|bp....PB|C
/// D|qp....PQ|D
/// E|kp....PK|E
/// F|bp....PB|F
/// G|np....PN|G
/// H|rp....PR|H
///  +--------+
///   12345678
std::vector<std::string> add_coordinats(
  const std::vector<std::string>& strs
);

/// Add a legend to the 8x8 or 12x12 strings of a chessboard.
///
/// Type  |White|Black
/// ------|-----|-----
/// bishop| b   | B
/// king  | k   | K
/// knight| n   | N
/// pawn  | p   | P
/// queen | q   | Q
/// rook  | r   | R
std::vector<std::string> add_legend(
  const std::vector<std::string>& strs
);

/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinate& coordinat
);

/// Can a piece move from 'from' to 'to'?
///
/// To do so:
/// - It must be able to do on an empty board
/// - There must be no piece in-between
/// - There must be no piece at the target
bool can_move(
  const std::vector<piece>& pieces,
  const chess_color placer_color,
  const piece_type selected_piece_type,
  const square& selected_piece_square,
  const square& destination_square
);

/// Collect the history of a game,
/// i.e. the moves played in time
action_history collect_action_history(const std::vector<piece>& pieces);

/// Count the total number of dead pieces
int count_dead_pieces(
  const std::vector<piece>& pieces
);

/// Count the total number of actions to be done by pieces of a player
int count_piece_actions(
  const std::vector<piece>& pieces,
  const chess_color player
);

std::vector<piece> create_pieces_from_fen_string(const fen_string& s);

/// Get the square where the king of that color is
square get_current_king_square(
  const std::vector<piece>& pieces,
  const chess_color c
);

/// Get a king-versus-king starting position
std::vector<piece> get_pieces_kings_only(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the maximum piece value a color can have on a standard chessboard
int get_max_pieces_value() noexcept;

/// Get all the squares that are occupied, disallowing duplicates
///
/// At the start and end of a tick, all occupied squares must be unique.
/// However, when updating each piece at a time, it can be that
/// some squares are duplicate untill all pieces are processed.
///
/// All squares are tested to be unique,
/// use \link{get_occupied_squares} to
/// allow duplicate squares.
std::vector<square> get_unique_occupied_squares(const std::vector<piece>& pieces);

/// Get all the squares that are occupied, allowing duplicates
///
/// At the start and end of a tick, all occupied squares must be unique.
/// However, when updating each piece at a time, it can be that
/// some squares are duplicate untill all pieces are processed.
///
/// All squares are not tested to be unique,
/// use \link{get_unique_occupied_squares} to
/// guarantee that all squares are unique.
std::vector<square> get_occupied_squares(const std::vector<piece>& pieces);

/// Get the piece that at that square,
/// will throw if there is no piece
const piece& get_piece_at(
  const std::vector<piece>& pieces,
  const square& coordinat
);

/// Get the piece that at that square,
/// will throw if there is no piece
piece& get_piece_at(
  std::vector<piece>& pieces,
  const square& coordinat
);

/// Get the possible moves for a focal piece that is a bishop.
/// This can both be a move or an attack
std::vector<square> get_possible_bishop_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
);

/// Get the possible moves for a focal piece that is a king.
/// This can both be a move or an attack
std::vector<square> get_possible_king_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
);

/// Get the possible moves for a focal piece that is a knight.
/// This can both be a move or an attack
std::vector<square> get_possible_knight_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
);

/// Get the possible moves for a focal piece.
/// This can both be a move or an attack
std::vector<square> get_possible_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
);

/// Get the possible moves for a focal piece that is a pawn.
/// This can both be a move or an attack
std::vector<square> get_possible_pawn_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
);

/// Get the possible moves for a focal piece that is a queen.
/// This can both be a move or an attack
std::vector<square> get_possible_queen_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
);

/// Get the possible moves for a focal piece that is a rook.
/// This can both be a move or an attack
std::vector<square> get_possible_rook_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
);



/// Get all the pieces in the starting position
std::vector<piece> get_standard_starting_pieces(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Create a setup of pieces in which en passant becomes possible
/// in the next move
std::vector<piece> get_pieces_before_en_passant(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces before a scholar's mate
/// 1. e4 e5
/// 2. Qh5 Nc6
/// 3. Bc4 Nf6??
/// (the checkmate is done by Qxf7#)
std::vector<piece> get_pieces_before_scholars_mate(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces from a bishop and knight end game
///  * White king: e6
///  * White knight: c4
///  * White bishop: g4
///  * Black king: d2 (note that it is in check)
/// From https://www.thechesswebsite.com/bishop-and-knight-end-game/
std::vector<piece> get_pieces_bishop_and_knight_end_game(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces for a Kasparov vs Topalov game,
/// discovered at https://www.reddit.com/r/chess/comments/fmd7uh/this_is_a_position_from_the_famous_kasparov_vs/
std::vector<piece> get_pieces_kasparov_vs_topalov(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces from a standard game, with all pawns moved two
/// squares forward
std::vector<piece> get_pieces_pawn_all_out_assault(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces where pawns are at promotion
///  * White king: e1
///  * White pawn: a8
///  * Black king: e8
///  * Black pawn: h1
std::vector<piece> get_pieces_pawns_at_promotion(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces where pawns are near promotion
///  * White king: e1
///  * White pawn: a7
///  * Black king: e8
///  * Black pawn: h2
std::vector<piece> get_pieces_pawns_near_promotion(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces where pawns are nealy near promotion
///  * White king: e1
///  * White pawn: a6
///  * Black king: e8
///  * Black pawn: h3
std::vector<piece> get_pieces_pawns_nearly_near_promotion(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces for a queen endgame,
/// which are having only the kings and queens left
/// from a default setup
std::vector<piece> get_pieces_queen_endgame(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces for a kings that are ready to castle
std::vector<piece> get_pieces_ready_to_castle(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the pieces for a kings that are ready to castle,
/// but cannot, as the king would go through check
std::vector<piece> get_pieces_ready_to_not_castle(
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get all the pieces in the starting position type
std::vector<piece> get_starting_pieces(
  const starting_position_type t = starting_position_type::standard,
  const race white_race = race::classic,
  const race black_race = race::classic
) noexcept;

/// Get the total value of all (non-king) chess pieces.
///
/// The king is not counted, as it has an infinite value.
int get_total_pieces_value(
  const std::vector<piece>& pieces,
  const chess_color c
);

/// Does this color still have a king?
bool has_king(
  const std::vector<piece>& pieces,
  const chess_color c
);

/// Determine if the player is mated
bool is_checkmate(
  const std::vector<piece>& pieces,
  const chess_color player_in_checkmate
);

/// Determine if the game is a draw
bool is_draw(const std::vector<piece>& pieces);

/// Determine if the king of the player is under attack
bool is_king_under_attack(
  const std::vector<piece>& pieces,
  const chess_color player_color
);

/// Determine if moving the piece at `from` to `to`
/// would leave its own king under attack,
/// as forbidden by rules 1.4.1b and 1.4.1c.
///
/// If there is an enemy piece at `to`, it is assumed to be captured.
/// If `captured_square` is given (e.g. for en-passant),
/// the piece at that square is assumed to be captured.
///
/// Returns false if the player has no king.
bool would_leave_king_under_attack(
  const std::vector<piece>& pieces,
  const square& from,
  const square& to,
  const std::optional<square>& captured_square = {}
);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const std::vector<piece>& pieces,
  const game_coordinate& coordinat,
  const double distance = 0.5
);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const std::vector<piece>& pieces,
  const square& coordinat
);

/// Determine if a piece (at `piece_square`) is looking at a target square
bool is_piece_looking_at_square(
  const std::vector<piece>& pieces,
  const square& piece_square,
  const square& target_square
);

/// Determine if the square is attacked by (another) piece of a certain color.
///
/// If the square is empty, it will determine if an enemy piece
/// is looking at this square.
///
/// If the square is occupied, it will determine if an enemy piece
/// is looking at that piece.
/// That piece can be of any color.
bool is_square_attacked(
  const std::vector<piece>& pieces,
  const square& s,
  const chess_color attacker_color
);

/// Determine if the square is protected by (another) piece of a certain color.
///
/// If the square is empty, it will determine if a friendly piece
/// is looking at this square.
///
/// If the square is occupied, it will determine if a friendly piece
/// is looking at that piece.
/// This function will assume that the square is occupied by a friendly piece.
bool is_square_protected(
  const std::vector<piece>& pieces,
  const square& s,
  const chess_color c
);

/// Test all these free functions
void test_pieces();

/// Show the pieces as if on a chessboard, such as:
///
/// Without coordinats:
///
/// rp....PR
/// np....PN
/// bp....PB
/// qp....PQ
/// kp....PK
/// bp....PB
/// np....PN
/// rp....PR
///
/// With coordinats:
///
///   12345678
///  +--------+
/// A|rp....PR|A
/// B|np....PN|B
/// C|bp....PB|C
/// D|qp....PQ|D
/// E|kp....PK|E
/// F|bp....PB|F
/// G|np....PN|G
/// H|rp....PR|H
///  +--------+
///   12345678
///
/// With legend:
///
/// Type  |Character when white|Character when black
/// ------|--------------------|--------------------
/// bishop| b                  | B
/// king  | k                  | K
/// knight| n                  | N
/// pawn  | p                  | P
/// queen | q                  | Q
/// rook  | r                  | R
///
std::string to_board_str(
  const std::vector<piece>& pieces,
  const board_to_text_options& options = board_to_text_options()
) noexcept;

/// Show the pieces as if on a chessboard
std::vector<std::string> to_board_strs(
  const std::vector<piece>& pieces,
  const board_to_text_options& options = board_to_text_options()
) noexcept;

/// Convert pieces to a FEN string.
///
/// This function does not attempt to be complete.
/// Instead, it is used to determine a checkmate,
/// using \url{https://github.com/Disservin/chess-library}
std::string to_fen_str(
  const std::vector<piece>& pieces,
  const chess_color active_color = chess_color::white,
  const std::string castling_availability = "KQkq",
  const std::string en_passant_target_square = "-",
  const int halfmove_clock = 0,
  const int fullmove_number = 1
);

/// Convert the played game (i.e. the action_history) to pseudo-PGN notation
/// Returns one string with newlines
std::string to_pgn(const std::vector<piece>& pieces);

/*
/// Unselect all pieces of a certain color
void unselect_all_pieces(
  std::vector<piece>& pieces,
  const chess_color color
);
*/

#endif // PIECES_H
