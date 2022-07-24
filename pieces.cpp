#include "pieces.h"

#include <algorithm>
#include <cassert>
#include <numeric>

std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
) {
  std::vector<double> distances;
  distances.reserve(pieces.size());
  std::transform(
    std::begin(pieces),
    std::end(pieces),
    std::back_inserter(distances),
    [coordinat](const auto& piece) {
      return calc_distance(
        coordinat,
        piece.get_coordinat()
      );
    }
  );
  assert(distances.size() == pieces.size());
  return distances;
}

int count_piece_actions(
  const std::vector<piece>& pieces,
  const chess_color player
)
{
  return std::accumulate(
    std::begin(pieces),
    std::end(pieces),
    0,
    [player](const int n, const auto& piece)
    {
      if (piece.get_color() == player)
      {
        return n + count_piece_actions(piece);
      }
      return n;
    }
  );
}

