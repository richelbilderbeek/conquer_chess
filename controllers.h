#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "chess_color.h"
#include "controller.h"
#include <vector>

// Work on more controllers


side get_keyboard_user_player_side(const std::vector<controller>& controllers);
side get_mouse_user_player_side(const std::vector<controller>& controllers);

std::vector<controller> get_default_controllers() noexcept;

#endif // CONTROLLERS_H
