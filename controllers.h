#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "chess_color.h"
#include "controller.h"
#include <vector>

// Work on more controllers

/// Count the number of controllers that use the keyboard
int count_keyboard_controllers(const std::vector<controller>& controllers) noexcept;

/// Count the number of controllers that use the keyboard
int count_mouse_controllers(const std::vector<controller>& controllers) noexcept;

/// Get a keyboard and a mouse controller
std::vector<controller> create_default_controllers() noexcept;

/// Get two keyboard controllers
std::vector<controller> create_two_keyboard_controllers() noexcept;

/// Get the side of the controller that uses the keyboard.
/// Assumes there is one controller that uses the keyboard
side get_keyboard_user_player_side(const std::vector<controller>& controllers);

/// Get the side of the controller that uses the mouse.
/// Assumes there is one controller that uses the mouse
side get_mouse_user_player_side(const std::vector<controller>& controllers);

/// Determine if there is at least one controller that use the keyboard
bool has_keyboard_controllers(const std::vector<controller>& controllers) noexcept;

/// Determine if there is at least one controller that use the mouse
bool has_mouse_controller(const std::vector<controller>& controllers) noexcept;

/// Test this class and its free functions
void test_controllers();

#endif // CONTROLLERS_H
