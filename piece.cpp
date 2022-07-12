#include "piece.h"

#include "piece_type.h"

piece::piece(
  const chess_color color,
  const piece_type type,
  const game_coordinat& coordinat
)
  : m_color{color},
    m_coordinat{coordinat},
    m_health{::get_max_health(type)},
    m_is_selected{false},
    m_max_health{::get_max_health(type)},
    m_type{type}
{

}

void piece::do_lmb_down()
{
  m_is_selected = !m_is_selected;
}

std::vector<piece> get_starting_pieces() noexcept
{
  return {
    piece(chess_color::white, piece_type::king, game_coordinat(4.0, 0.0)),
    piece(chess_color::black, piece_type::king, game_coordinat(4.0, 7.0))
  };
}
