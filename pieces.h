#ifndef PIECES_H
#define PIECES_H

/// Functions to work on collections of pieces
#include "piece.h"


/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
);

/// Count the total number of dead pieces
int count_dead_pieces(
  const std::vector<piece>& pieces
);

/// Count the total number of actions to be done by pieces of a player
int count_piece_actions(
  const std::vector<piece>& pieces,
  const chess_color player
);

/// Count the number of selected units of a player
int count_selected_units(
  const std::vector<piece>& pieces,
  const chess_color player
);

/// Get a king-versus-king starting position
std::vector<piece> get_kings_only_starting_pieces(
  const chess_color left_player_color = chess_color::white
) noexcept;

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

/// Get all the selected pieces
/// @param player the color of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const std::vector<piece>& pieces,
  const chess_color player
);

/// Get all the pieces in the starting position
std::vector<piece> get_standard_starting_pieces(
  const chess_color left_player_color = chess_color::white
) noexcept;

/// Get the pieces before a scholar's mate
/// 1. e4 e5
/// 2. Qh5 Nc6
/// 3. Bc4 Nf6??
/// (the checkmate is done by Qxf7#)
std::vector<piece> get_pieces_before_scholars_mate(
  const chess_color left_player_color = chess_color::white
) noexcept;

/// Get the pieces from a bishop and knight end game
///  * White king: e6
///  * White knight: c4
///  * White bishop: g4
///  * Black king: d2 (note that it is in check)
/// From https://www.thechesswebsite.com/bishop-and-knight-end-game/
std::vector<piece> get_pieces_bishop_and_knight_end_game(
  const chess_color left_player_color = chess_color::white
) noexcept;

/// Get the pieces from a standard game, with all pawns moved two
/// squares forward
std::vector<piece> get_pieces_pawn_all_out_assault(
  const chess_color left_player_color = chess_color::white
) noexcept;


/// Get all the pieces in the starting position type
std::vector<piece> get_starting_pieces(
  const starting_position_type t,
  const chess_color left_player_color = chess_color::white
) noexcept;

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

/// Unselect all pieces of a certain color
void unselect_all_pieces(
  std::vector<piece>& pieces,
  const chess_color color
);

#endif // PIECES_H
