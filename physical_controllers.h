#ifndef PHYSICAL_CONTROLLERS_H
#define PHYSICAL_CONTROLLERS_H

#include "chess_color.h"
#include "physical_controller.h"
#include <vector>

/// All the physical controllers
class physical_controllers
{
public:

  physical_controllers(
    const std::vector<physical_controller> controllers = {
      create_left_keyboard_controller(),
      create_default_mouse_controller()
    }
  );

  /// Get the controller of a player
  const physical_controller& get_controller(const side player_side) const noexcept;

  /// Get the physical controllers
  const std::vector<physical_controller>& get_controllers() const noexcept;

  /// Process the input for this controller.
  /// Returns an an empty vector if this controller is uneffected by the event
  //user_inputs process_input(
  //  const sf::Event& event,
  //  const game& g
  //) const;

  /// Set a controller
  void set(const side player_side, const physical_controller& controller);

private:

  std::vector<physical_controller> m_physical_controllers;
};

/// Count the number of controllers that use the keyboard
int count_keyboard_controllers(const physical_controllers& controllers) noexcept;

/// Count the number of controllers that use the keyboard
int count_mouse_controllers(const physical_controllers& controllers) noexcept;

/// Count the number of controllers
int count_n_controllers(const physical_controllers& controllers) noexcept;

/// Get a keyboard and a mouse controller.
/// @see use \link{create_two_keyboard_controllers}
/// to create two keyboard controllers
physical_controllers create_default_controllers() noexcept;

/// Get two controllers,
/// where LHS player uses keyboard,
/// and RHS player uses mouse
physical_controllers create_keyboard_mouse_controllers() noexcept;

/// Get two controllers,
/// where LHS player uses mouse,
/// and RHS player uses keyboard
physical_controllers create_mouse_keyboard_controllers() noexcept;

/// Get two keyboard controllers.
/// @see use \link{create_default_controllers}
/// to create the defaults one-keyboard-one-mouse controllers
physical_controllers create_two_keyboard_controllers() noexcept;

/// Get the side of the controller that uses the keyboard.
/// Assumes there is one controller that uses the keyboard
side get_keyboard_user_player_side(const physical_controllers& controllers);

/// Get the side of the controller that uses the mouse.
/// Assumes there is one controller that uses the mouse
side get_mouse_user_player_side(const physical_controllers& controllers);

/// Determine if there is at least one controller that use the keyboard
bool has_keyboard_controller(const physical_controllers& controllers) noexcept;

/// Determine if there is at least one controller that use the mouse
bool has_mouse_controller(const physical_controllers& controllers) noexcept;

/// Test this class and its free functions
void test_physical_controllers();

bool operator==(const physical_controllers& lhs, const physical_controllers& rhs) noexcept;

#endif // PHYSICAL_CONTROLLERS_H
