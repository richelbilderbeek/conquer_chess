#ifndef CHESS_COLOR_H
#define CHESS_COLOR_H

enum class chess_color
{
  black, white
};

/// Get the other color
chess_color get_other_color(const chess_color c);

/// Test the chess_color functions
void test_chess_color();

#endif // CHESS_COLOR_H
