#ifndef CONTROLLER_TYPE_H
#define CONTROLLER_TYPE_H

#include <string>

/// The type of controller
enum class controller_type
{
  keyboard,
  mouse
};

std::string to_str(const controller_type t) noexcept;

#endif // CONTROLLER_TYPE_H
