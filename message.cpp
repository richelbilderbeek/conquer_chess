#include "message.h"

#include <sstream>

message::message(
  const message_type set,
  const chess_color c,
  const piece_type pc
) : m_message_type{set},
    m_chess_color{c},
    m_piece_type{pc}
{

}

std::string to_str(const message& m) noexcept
{
  std::stringstream s;
  switch (m.get_message_type())
  {
    case message_type::cannot:
      s << m.get_color() << " " << m.get_piece_type() << " cannot do that";
      break;
    case message_type::select:
      s << m.get_color() << " " << m.get_piece_type() << " selected";
      break;
    case message_type::start_move:
      s << m.get_color() << " " << m.get_piece_type() << " starts moving";
      break;
    case message_type::start_attack:
      s << m.get_color() << " " << m.get_piece_type() << " starts attacking";
      break;
  }
  return s.str();
}
