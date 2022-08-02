#include "sfml_helper.h"

//#define HAS_THOR
#ifdef HAS_THOR
#include <Thor/Input/InputNames.hpp>
#endif

//#include "../magic_enum/include/magic_enum.hpp"

#ifndef LOGIC_ONLY

#include <cassert>
#include <cmath>

sf::Color f_health_to_color(const double f)
{
  assert(f >= 0.0);
  assert(f <= 1.0);
  if (f < 0.25) return sf::Color::Red;
  if (f < 0.50) return sf::Color(255, 128, 0);
  if (f < 0.75) return sf::Color::Yellow;
  return sf::Color::Green;
}

void set_rect(sf::RectangleShape& rectangle, const screen_rect& screen_rect)
{
  rectangle.setOrigin(
    get_width(screen_rect) / 2,
    get_height(screen_rect) / 2
  );
  rectangle.setPosition(
    get_center(screen_rect).get_x(),
    get_center(screen_rect).get_y()
  );
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
}

void set_text_position(sf::Text& text, const screen_rect& screen_rect)
{
  assert(text.getString().getSize() > 0);
  // https://en.sfml-dev.org/forums/index.php?topic=15875.msg113439#msg113439
  text.setPosition(
    get_center(screen_rect).get_x(),
    get_center(screen_rect).get_y()
  );
  // https://en.sfml-dev.org/forums/index.php?topic=15875.msg113439#msg113439
  text.setOrigin(
    std::round(text.getLocalBounds().width / 2.0),
    std::round(text.getLocalBounds().height / 2.0)
  );
}

sf::Color to_sfml_color(const chess_color color) noexcept
{
  if (color == chess_color::white) return sf::Color::White;
  assert(color == chess_color::black);
  return sf::Color::Black;
}

std::string to_str(const sf::Keyboard::Key k)
{
  #ifdef HAS_THOR
  return thor::toString(k);
  #else
  switch (k)
  {
    case sf::Keyboard::Key::A: return "A";
    case sf::Keyboard::Key::Add: return "Add";
    case sf::Keyboard::Key::B: return "B";
    case sf::Keyboard::Key::Backslash: return "Backslash";
    case sf::Keyboard::Key::Backspace: return "Backspace";
    case sf::Keyboard::Key::C: return "C";
    case sf::Keyboard::Key::Comma: return "Comma";
    case sf::Keyboard::Key::D: return "D";
    case sf::Keyboard::Key::Delete: return "Delete";
    case sf::Keyboard::Key::Divide: return "Divide";
    case sf::Keyboard::Key::Down: return "Down";
    case sf::Keyboard::Key::E: return "E";
    case sf::Keyboard::Key::End: return "End";
    case sf::Keyboard::Key::Enter: return "Enter";
    case sf::Keyboard::Key::Equal: return "Equal";
    case sf::Keyboard::Key::Escape: return "Escape";
    case sf::Keyboard::Key::F10: return "F10";
    case sf::Keyboard::Key::F11: return "F11";
    case sf::Keyboard::Key::F12: return "F12";
    case sf::Keyboard::Key::F13: return "F13";
    case sf::Keyboard::Key::F14: return "F14";
    case sf::Keyboard::Key::F15: return "F15";
    case sf::Keyboard::Key::F1: return "F1";
    case sf::Keyboard::Key::F2: return "F2";
    case sf::Keyboard::Key::F3: return "F3";
    case sf::Keyboard::Key::F4: return "F4";
    case sf::Keyboard::Key::F5: return "F5";
    case sf::Keyboard::Key::F6: return "F6";
    case sf::Keyboard::Key::F7: return "F7";
    case sf::Keyboard::Key::F8: return "F8";
    case sf::Keyboard::Key::F9: return "F9";
    case sf::Keyboard::Key::F: return "F";
    case sf::Keyboard::Key::G: return "G";
    case sf::Keyboard::Key::H: return "H";
    case sf::Keyboard::Key::Home: return "Home";
    case sf::Keyboard::Key::Hyphen: return "Hyphen";
    case sf::Keyboard::Key::I: return "I";
    case sf::Keyboard::Key::Insert: return "Insert";
    case sf::Keyboard::Key::J: return "J";
    case sf::Keyboard::Key::K: return "K";
    case sf::Keyboard::Key::KeyCount: return "KeyCount";
    case sf::Keyboard::Key::L: return "L";
    case sf::Keyboard::Key::LAlt: return "LAlt";
    case sf::Keyboard::Key::LBracket: return "LBracket";
    case sf::Keyboard::Key::LControl: return "LControl";
    case sf::Keyboard::Key::LShift: return "LShift";
    case sf::Keyboard::Key::LSystem: return "LSystem";
    case sf::Keyboard::Key::Left: return "Left";
    case sf::Keyboard::Key::M: return "M";
    case sf::Keyboard::Key::Menu: return "Menu";
    case sf::Keyboard::Key::Multiply: return "Multiply";
    case sf::Keyboard::Key::N: return "N";
    case sf::Keyboard::Key::Num0: return "Num0";
    case sf::Keyboard::Key::Num1: return "Num1";
    case sf::Keyboard::Key::Num2: return "Num2";
    case sf::Keyboard::Key::Num3: return "Num3";
    case sf::Keyboard::Key::Num4: return "Num4";
    case sf::Keyboard::Key::Num5: return "Num5";
    case sf::Keyboard::Key::Num6: return "Num6";
    case sf::Keyboard::Key::Num7: return "Num7";
    case sf::Keyboard::Key::Num8: return "Num8";
    case sf::Keyboard::Key::Num9: return "Num9";
    case sf::Keyboard::Key::Numpad0: return "Numpad0";
    case sf::Keyboard::Key::Numpad1: return "Numpad1";
    case sf::Keyboard::Key::Numpad2: return "Numpad2";
    case sf::Keyboard::Key::Numpad3: return "Numpad3";
    case sf::Keyboard::Key::Numpad4: return "Numpad4";
    case sf::Keyboard::Key::Numpad5: return "Numpad5";
    case sf::Keyboard::Key::Numpad6: return "Numpad6";
    case sf::Keyboard::Key::Numpad7: return "Numpad7";
    case sf::Keyboard::Key::Numpad8: return "Numpad8";
    case sf::Keyboard::Key::Numpad9: return "Numpad9";
    case sf::Keyboard::Key::O: return "O";
    case sf::Keyboard::Key::P: return "P";
    case sf::Keyboard::Key::PageDown: return "PageDown";
    case sf::Keyboard::Key::PageUp: return "PageUp";
    case sf::Keyboard::Key::Pause: return "Pause";
    case sf::Keyboard::Key::Period: return "Period";
    case sf::Keyboard::Key::Q: return "Q";
    case sf::Keyboard::Key::Quote: return "Quote";
    case sf::Keyboard::Key::R: return "R";
    case sf::Keyboard::Key::RAlt: return "RAlt";
    case sf::Keyboard::Key::RBracket: return "RBracket";
    case sf::Keyboard::Key::RControl: return "RControl";
    case sf::Keyboard::Key::RShift: return "RShift";
    case sf::Keyboard::Key::RSystem: return "RSystem";
    case sf::Keyboard::Key::Right: return "Right";
    case sf::Keyboard::Key::S: return "S";
    case sf::Keyboard::Key::Semicolon: return "Semicolon";
    case sf::Keyboard::Key::Slash: return "Slash";
    case sf::Keyboard::Key::Space: return "Space";
    case sf::Keyboard::Key::Subtract: return "Subtract";
    case sf::Keyboard::Key::T: return "T";
    case sf::Keyboard::Key::Tab: return "Tab";
    case sf::Keyboard::Key::Tilde: return "Tilde";
    case sf::Keyboard::Key::U: return "U";
    case sf::Keyboard::Key::Unknown: return "Unknown";
    case sf::Keyboard::Key::Up: return "Up";
    case sf::Keyboard::Key::V: return "V";
    case sf::Keyboard::Key::W: return "W";
    case sf::Keyboard::Key::X: return "X";
    case sf::Keyboard::Key::Y: return "Y";
    case sf::Keyboard::Key::Z:
    default:
      assert(k == sf::Keyboard::Key::Z);
      return "Z";
  }
  #endif
}

#endif // LOGIC_ONLY
