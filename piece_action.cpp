#include "piece_action.h"

#include <iostream>
#include <sstream>

piece_action::piece_action(
  const piece_action_type type,
  const game_coordinat& coordinat
) : m_type{type}, m_coordinat{coordinat}
{

}

std::string describe_action(const piece_action& p)
{
  std::stringstream s;
  s << p;
  return s.str();
}

std::ostream& operator<<(std::ostream& os, const piece_action& p)
{
  os << p.get_type() << " to " << p.get_coordinat();
  return os;
}
