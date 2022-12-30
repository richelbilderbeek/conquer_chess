#ifndef SCREEN_RECT_H
#define SCREEN_RECT_H

#include "screen_coordinat.h"

#include <iosfwd>

/// A rectangle, as down on the screen
class screen_rect
{
public:
  explicit screen_rect(
    const screen_coordinat& top_left = screen_coordinat(),
    const screen_coordinat& bottom_right = screen_coordinat()
  );

  const auto& get_tl() const noexcept { return m_top_left; }
  const auto& get_br() const noexcept { return m_bottom_right; }

private:
  screen_coordinat m_top_left;
  screen_coordinat m_bottom_right;
};


screen_coordinat get_center(const screen_rect& r) noexcept;

/// Get the bottom-left corner (i.e. 25% of the rect)
screen_rect get_bottom_left_corner(const screen_rect& r) noexcept;

/// Get the bottom-right corner (i.e. 25% of the rect)
screen_rect get_bottom_right_corner(const screen_rect& r) noexcept;

int get_height(const screen_rect& r) noexcept;

/// Get the lower half (i.e. 50% of the rect)
screen_rect get_lower_half(const screen_rect& r) noexcept;

/// Get the top-left corner (i.e. 25% of the rect)
screen_rect get_top_left_corner(const screen_rect& r) noexcept;

/// Get the top-right corner (i.e. 25% of the rect)
screen_rect get_top_right_corner(const screen_rect& r) noexcept;

/// Get the upper half (i.e. 50% of the rect)
screen_rect get_upper_half(const screen_rect& r) noexcept;

int get_width(const screen_rect& r) noexcept;

/// Is the coordinat in the rectangle?
bool is_in(const screen_coordinat& pos, const screen_rect& r) noexcept;

/// Test this class and its free functions
void test_screen_rect();

bool operator==(const screen_rect& lhs, const screen_rect& rhs) noexcept;
bool operator!=(const screen_rect& lhs, const screen_rect& rhs) noexcept;
screen_rect& operator+=(screen_rect& rect, const screen_coordinat& delta) noexcept;

std::ostream& operator<<(std::ostream& os, const screen_rect& r) noexcept;

#endif // SCREEN_RECT_H
