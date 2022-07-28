#ifndef MESSAGE_H
#define MESSAGE_H

#include "message_type.h"
#include "chess_color.h"
#include "piece_type.h"

#include <iosfwd>

/// Representation of a sound effect
/// Use 'MESSAGEs::play' to play it
class message
{
public:
  explicit message(
    const message_type set,
    const chess_color c,
    const piece_type pc
  );
  auto get_message_type() const noexcept { return m_message_type; }
  auto get_color() const noexcept { return m_chess_color; }
  auto get_piece_type() const noexcept { return m_piece_type; }
  private:

  message_type m_message_type;
  chess_color m_chess_color;
  piece_type m_piece_type;
};

/// Test this class and its free functions
void test_message();

std::string to_str(const message& m) noexcept;

std::ostream& operator<<(std::ostream& os, const message& m) noexcept;

#endif // MESSAGE_H
