#ifndef PHYSICAL_CONTROLLERS_H
#define PHYSICAL_CONTROLLERS_H

#include "chess_color.h"
#include "physical_controller.h"
#include <vector>

/// All the physical controllers
class physical_controllers
{
  physical_controllers(
    const std::vector<physical_controller> controllers = {}
  );

  private:

  std::vector<physical_controller> m_physical_controllers;
};

/// Count the number of controllers that use the keyboard
int count_keyboard_controllers(const std::vector<physical_controller>& controllers) noexcept;

/// Count the number of controllers that use the keyboard
int count_mouse_controllers(const std::vector<physical_controller>& controllers) noexcept;

/// Get a keyboard and a mouse controller.
/// @see use \link{create_two_keyboard_controllers}
/// to create two keyboard controllers
std::vector<physical_controller> create_default_controllers() noexcept;

/// Get two controllers,
/// where LHS player uses keyboard,
/// and RHS player uses mouse
std::vector<physical_controller> create_keyboard_mouse_controllers() noexcept;

/// Get two controllers,
/// where LHS player uses mouse,
/// and RHS player uses keyboard
std::vector<physical_controller> create_mouse_keyboard_controllers() noexcept;

/// Get two keyboard controllers.
/// @see use \link{create_default_controllers}
/// to create the defaults one-keyboard-one-mouse controllers
std::vector<physical_controller> create_two_keyboard_controllers() noexcept;

/// Get the side of the controller that uses the keyboard.
/// Assumes there is one controller that uses the keyboard
side get_keyboard_user_player_side(const std::vector<physical_controller>& controllers);

/// Get the side of the controller that uses the mouse.
/// Assumes there is one controller that uses the mouse
side get_mouse_user_player_side(const std::vector<physical_controller>& controllers);

/// Determine if there is at least one controller that use the keyboard
bool has_keyboard_controller(const std::vector<physical_controller>& controllers) noexcept;

/// Determine if there is at least one controller that use the mouse
bool has_mouse_controller(const std::vector<physical_controller>& controllers) noexcept;

/// Test this class and its free functions
void test_controllers();

#endif // PHYSICAL_CONTROLLERS_H
