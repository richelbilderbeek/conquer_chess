#include "game_view_layout.h"

#include <cmath>

game_view_layout::game_view_layout(
  const screen_coordinat& window_size
) : m_window_size{window_size}
{
  const int tl_margin_width{2};
  const int tl_margin_height{2};
  const int center_margin_width{10};
  m_tl_board = screen_coordinat(tl_margin_width, tl_margin_height);
  const int max_board_height = m_window_size.get_y() - tl_margin_height - tl_margin_height;
  const int max_board_width = m_window_size.get_x() - tl_margin_width - center_margin_width;
  const int board_width = std::min(max_board_height, max_board_width);
  const int board_height = board_width;
  m_br_board = screen_coordinat(tl_margin_width + board_width, tl_margin_height + board_height);
  m_tl_side = screen_coordinat(tl_margin_width + board_width + center_margin_width, tl_margin_height);
  m_br_side = screen_coordinat(
    m_window_size.get_x() - tl_margin_width,
    m_window_size.get_y() - tl_margin_height
  );
}

double game_view_layout::get_square_height() const noexcept
{
  return static_cast<double>(m_br_board.get_x() - m_tl_board.get_x()) / 8.0;
}

double game_view_layout::get_square_width() const noexcept
{
  return static_cast<double>(m_br_board.get_y() - m_tl_board.get_y()) / 8.0;
}
