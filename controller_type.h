#ifndef CONTROLLER_TYPE_H
#define CONTROLLER_TYPE_H

#include <string>

/// The type of controller
enum class controller_type
{
  keyboard,
  mouse
};

/// Test this class and its free functions
void test_controller_type();

std::string to_str(const controller_type t) noexcept;

#endif // CONTROLLER_TYPE_H
