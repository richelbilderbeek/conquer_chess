#ifndef CHESS_COLOR_H
#define CHESS_COLOR_H

#include <iosfwd>
#include <string>
#include <vector>

enum class chess_color
{
  black, white
};

std::vector<chess_color> get_all_chess_colors() noexcept;

/// Get the other color
chess_color get_other_color(const chess_color c) noexcept;

/// Test the chess_color functions
void test_chess_color();

std::string to_str(const chess_color c) noexcept;

std::ostream& operator<<(std::ostream& os, const chess_color c) noexcept;

#endif // CHESS_COLOR_H
