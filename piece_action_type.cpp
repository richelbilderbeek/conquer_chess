#include "piece_action_type.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const piece_action_type& p)
{
  switch (p)
  {
    case piece_action_type::move: os << "move"; break;
    case piece_action_type::attack: os << "attack"; break;
  }
  return os;
}
