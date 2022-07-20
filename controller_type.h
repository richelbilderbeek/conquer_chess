#ifndef CONTROLLER_TYPE_H
#define CONTROLLER_TYPE_H

#include <iosfwd>
#include <string>

/// The type of controller
enum class controller_type
{
  keyboard,
  mouse
};

/// Get the next controller type,
/// i.e. when the player presses right
controller_type get_next(const controller_type t) noexcept;

/// Test this class and its free functions
void test_controller_type();

std::string to_str(const controller_type t) noexcept;

std::ostream& operator<<(std::ostream& os, const controller_type t) noexcept;

#endif // CONTROLLER_TYPE_H
