#include "piece.h"

#include "piece_type.h"

piece::piece(
  const chess_color color,
  const piece_type type,
  const double x,
  const double y
)
  : m_color{color},
    m_f_health{1.0},
    m_type{type},
    m_x{x},
    m_y{y}
{

}

std::vector<piece> get_starting_pieces() noexcept
{
  return {
    piece(chess_color::white, piece_type::king, 4.0, 0.0),
    piece(chess_color::black, piece_type::king, 4.0, 7.0)
  };
}
