#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "key_bindings.h"
#include "control_action.h"
#include "controller_type.h"

/// A controller is a class that takes input from
/// a mouse/keyboard/AI and converts these into
/// the right 'control_action'.
/// sf::Event -> controllers -> control_action
class controller
{
public:
  explicit controller(
    const controller_type type,
    const side player,
    const key_bindings& ks
  );

  /// Key binding, only used when the type is a keyboard
  const auto& get_key_bindings() const noexcept { return m_key_bindings; }

  /// Get the side this controller is on
  side get_player() const noexcept { return m_player; }

  const controller_type& get_type() const noexcept { return m_type; }

  /// Process the input for this controller.
  /// Returns an an empty vector if this controller is uneffected by the event

  std::vector<control_action> process_input(
   const sf::Event& event,
   const game& g
 ) const;

  void set_type(const controller_type t) noexcept { m_type = t; }

private:

  /// Key binding, only used when the type is a keyboard
  key_bindings m_key_bindings;

  side m_player;

  controller_type m_type;

  /// Process a key press from a keyboard
  std::vector<control_action> process_key_press(
    const sf::Event& event
  ) const;

  /// Process the input from a mouse button press
  std::vector<control_action> process_mouse_pressed(
    const sf::Event& event,
    const game& g
  ) const;

  /// Process the input from a mouse movement
  std::vector<control_action> process_mouse_moved(
    const sf::Event& event,
    const game& g
  ) const;
};

/// Create a default keyboard controller,
/// which is the left keyboard controller
controller create_default_keyboard_controller(const side player) noexcept;

/// Create a mouse controller
controller create_default_mouse_controller(const side player) noexcept;

/// Create an sf::Event with type sf::Event::KeyPressed
sf::Event create_key_pressed_event(const sf::Keyboard::Key k);

/// Create a keyboard controller
/// for a user at the left side of the keyboard
controller create_left_keyboard_controller(const side player) noexcept;

/// Create a keyboard controller
/// for a user at the right side of the keyboard
controller create_right_keyboard_controller(const side player) noexcept;

/// Get the key for a action 1, 2, 3 or 4 for a controller
sf::Keyboard::Key get_key_for_action(const controller& c, const int action);

/// Get the text for action 1, 2, 3 or 4, e.g.
/// 'Q\nSelect' for a keyboard player that has nothing selected
/// 'E\nAttack' for a keyboard player that has a piece select
/// 'LMB\nSelect' for a mouse player that has nothing selected'
std::string get_text_for_action(
  const controller& c,
  const bool has_selected_units,
  const int action_key_number
) noexcept;

/// Test this class and its free functions
void test_controller();

bool operator==(const controller& lhs, const controller& rhs) noexcept;
bool operator!=(const controller& lhs, const controller& rhs) noexcept;

#endif // CONTROLLER_H
