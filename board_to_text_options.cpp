#include "board_to_text_options.h"

#include <cassert>

board_to_text_options::board_to_text_options(
  const bool shows_coordinats,
  const bool shows_legend
) noexcept
  : m_show_coordinats{shows_coordinats},
    m_show_legend{shows_legend}
{

}

void test_board_to_text_options()
{
#ifndef NDEBUG
  {
    const board_to_text_options h;
    assert(!h.get_show_coordinats());
    assert(!h.get_show_legend());
  }
#endif
}
