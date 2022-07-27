#include "message.h"

message::message(
  const message_type set,
  const chess_color c,
  const piece_type pc
) : m_message_type{set},
    m_chess_color{c},
    m_piece_type{pc}
{

}
