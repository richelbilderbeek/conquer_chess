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


#endif // PIECES_H
