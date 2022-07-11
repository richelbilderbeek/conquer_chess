#ifndef ACTION_H
#define ACTION_H

#include "action_type.h"
#include "screen_coordinat.h"

/// An action
class action
{
public:
  action(const action_type type, const screen_coordinat& coordinat);

private:
  screen_coordinat m_coordinat;
  action_type m_type;
};

action create_mouse_move_action(const screen_coordinat& coordinat);
action create_press_lmb_action(const screen_coordinat& coordinat);


#endif // ACTION_H

