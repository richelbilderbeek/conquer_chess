#include "message.h"

#include <cassert>
#include <iostream>
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

void test_message()
{
#ifndef NDEBUG
  // message::message and message::get_x
  {
    const message_type mt{message_type::start_attack};
    const chess_color c{chess_color::black};
    const piece_type pt{piece_type::bishop};
    const message m(mt, c, pt);
    assert(m.get_color() == c);
    assert(m.get_piece_type() == pt);
    assert(m.get_message_type() == mt);
  }
  // to_str
  {
    assert(!to_str(message(message_type::cannot, chess_color::white, piece_type::king)).empty());
    assert(!to_str(message(message_type::select, chess_color::white, piece_type::king)).empty());
    assert(!to_str(message(message_type::start_move, chess_color::white, piece_type::king)).empty());
    assert(!to_str(message(message_type::start_attack, chess_color::white, piece_type::king)).empty());
  }
  // operator<<
  {
    std::stringstream s;
    s << message(message_type::cannot, chess_color::white, piece_type::king);
    assert(!s.str().empty());
  }
#endif // NDEBUG
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

std::ostream& operator<<(std::ostream& os, const message& m) noexcept
{
  os << to_str(m);
  return os;
}
