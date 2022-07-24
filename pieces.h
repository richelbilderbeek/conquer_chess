#ifndef PIECES_H
#define PIECES_H

/// Functions to work on collections of pieces
#include "piece.h"


/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
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

/// Get all the selected pieces
/// @param player the color of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const std::vector<piece>& pieces,
  const chess_color player
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

/// Unselect all pieces of a certain color
void unselect_all_pieces(
  std::vector<piece>& pieces,
  const chess_color color
);

#endif // PIECES_H
