#ifndef KEY_BINDINGS_H
#define KEY_BINDINGS_H

#include <SFML/Window/Keyboard.hpp>

/// Which key does which action
class key_bindings
{
public:
  key_bindings();

  /// Get the key for action 1, 2, 3 or 4.
  sf::Keyboard::Key get_key_for_action(const int action) const noexcept;

  /// Get the key for moving down
  sf::Keyboard::Key get_key_for_move_down() const noexcept;

  /// Get the key for moving left
  sf::Keyboard::Key get_key_for_move_left() const noexcept;

  /// Get the key for moving right
  sf::Keyboard::Key get_key_for_move_right() const noexcept;

  /// Get the key for moving up
  sf::Keyboard::Key get_key_for_move_up() const noexcept;
};

/// Test this class and its free functions
void test_key_bindings();

#endif // KEY_BINDINGS_H
