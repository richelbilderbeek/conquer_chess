#ifndef PIECES_H
#define PIECES_H

/// Functions to work on collections of pieces
#include "piece.h"


/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
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

/// Count the number of selected units for both players
int count_selected_units(
  const std::vector<piece>& pieces
);

/// Count the number of selected units of a color
int count_selected_units(
  const std::vector<piece>& pieces,
  const chess_color player
);

/// Get a king-versus-king starting position
std::vector<piece> get_kings_only_starting_pieces() noexcept;

/// Get all the squares that are occupied
std::vector<square> get_occupied_squares(const std::vector<piece>& pieces) noexcept;

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

/// Find a piece with a certain ID
/// Will throw if there is no piece with that ID
piece get_piece_with_id(
  const std::vector<piece>& pieces,
  const id& i
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

/// Get all the selected pieces
/// @param player the color of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const std::vector<piece>& pieces,
  const chess_color player
);

/// Get all the pieces in the starting position
std::vector<piece> get_standard_starting_pieces() noexcept;


/// Get the pieces before an en passant becomes possible
std::vector<piece> get_pieces_before_en_passant() noexcept;

/// Get the pieces before a scholar's mate
/// 1. e4 e5
/// 2. Qh5 Nc6
/// 3. Bc4 Nf6??
/// (the checkmate is done by Qxf7#)
std::vector<piece> get_pieces_before_scholars_mate() noexcept;

/// Get the pieces from a bishop and knight end game
///  * White king: e6
///  * White knight: c4
///  * White bishop: g4
///  * Black king: d2 (note that it is in check)
/// From https://www.thechesswebsite.com/bishop-and-knight-end-game/
std::vector<piece> get_pieces_bishop_and_knight_end_game() noexcept;

/// Get the pieces for a Kasparov vs Topalov game,
/// discovered at https://www.reddit.com/r/chess/comments/fmd7uh/this_is_a_position_from_the_famous_kasparov_vs/
std::vector<piece> get_pieces_kasparov_vs_topalov() noexcept;

/// Get the pieces from a standard game, with all pawns moved two
/// squares forward
std::vector<piece> get_pieces_pawn_all_out_assault() noexcept;

/// Get the pieces where pawns are at promotion
///  * White king: e1
///  * White pawn: a8
///  * Black king: e8
///  * Black pawn: h1
std::vector<piece> get_pieces_pawns_at_promotion() noexcept;

/// Get the pieces where pawns are near promotion
///  * White king: e1
///  * White pawn: a7
///  * Black king: e8
///  * Black pawn: h2
std::vector<piece> get_pieces_pawns_near_promotion() noexcept;

/// Get the pieces where pawns are nealy near promotion
///  * White king: e1
///  * White pawn: a6
///  * Black king: e8
///  * Black pawn: h3
std::vector<piece> get_pieces_pawns_nearly_near_promotion() noexcept;

/// Get the pieces for a queen endgame,
/// which are having only the kings and queens left
/// from a default setup
std::vector<piece> get_pieces_queen_endgame() noexcept;

/// Get the pieces for a kings that are ready to castle
std::vector<piece> get_pieces_ready_to_castle() noexcept;

/// Get the pieces for a kings that are ready to castle,
/// but cannot, as the king would go through check
std::vector<piece> get_pieces_ready_to_not_castle() noexcept;

/// Get all the pieces in the starting position type
std::vector<piece> get_starting_pieces(
  const starting_position_type t
) noexcept;

/// Is there a piece with the ID among the pieces?
bool has_piece_with_id(
  const std::vector<piece>& pieces,
  const id& i
);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat,
  const double distance = 0.5
);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const std::vector<piece>& pieces,
  const square& coordinat
);

/// Test all these free functions
void test_pieces();

/// Show the pieces as if on a chessboard, such as:
/// rp....PR
/// np....PN
/// bp....PB
/// qp....PQ
/// kp....PK
/// bp....PB
/// np....PN
/// rp....PR
std::string to_board_str(const std::vector<piece>& pieces) noexcept;

/// Show the pieces as if on a chessboard
std::vector<std::string> to_board_strs(const std::vector<piece>& pieces) noexcept;

/// Convert the played game (i.e. the action_history) to pseudo-PGN notation
/// Returns one string with newlines
std::string to_pgn(const std::vector<piece>& pieces);

/// Unselect all pieces of a certain color
void unselect_all_pieces(
  std::vector<piece>& pieces,
  const chess_color color
);

#endif // PIECES_H
