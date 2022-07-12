#ifndef PIECE_ACTION_H
#define PIECE_ACTION_H

#include "piece_action_type.h"
#include "game_coordinat.h"

/// An action to be done by a piece
class piece_action
{
public:
  piece_action(
    const piece_action_type type,
    const game_coordinat& coordinat
  );

  const auto& get_coordinat() const noexcept { return m_coordinat; }
  auto get_type() const noexcept { return m_type; }

private:

  piece_action_type m_type;
  game_coordinat m_coordinat;
};

#endif // PIECE_ACTION_H
