#ifndef CHESS_COLOR_H
#define CHESS_COLOR_H

#include <iosfwd>
#include <string>

enum class chess_color
{
  black, white
};

/// Get the other color
chess_color get_other_color(const chess_color c) noexcept;

/// Test the chess_color functions
void test_chess_color();

std::string to_str(const chess_color c) noexcept;

std::ostream& operator<<(std::ostream& os, const chess_color c) noexcept;

#endif // CHESS_COLOR_H
