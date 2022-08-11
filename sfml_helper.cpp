#include "sfml_helper.h"

#include "game_resources.h"

#include <SFML/Graphics/RectangleShape.hpp>

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

void show_squares(
  sf::RenderWindow& window,
  const screen_rect& rect,
  game_resources& resources
)
{
  const int square_width{1 + (get_width(rect) / 8)};
  const int square_height{1 + (get_height(rect) / 8)};
  sf::RectangleShape black_square;
  black_square.setSize(sf::Vector2f(square_width, square_height));
  black_square.setTexture(&resources.get_square(chess_color::black));
  black_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));

  sf::RectangleShape white_square;
  white_square.setSize(sf::Vector2f(square_width, square_height));
  white_square.setTexture(&resources.get_square(chess_color::white));
  white_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));

  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      sf::RectangleShape& s = (x + y) % 2 == 0 ? black_square : white_square;
      const screen_coordinat square_pos{
        static_cast<int>(
          rect.get_tl().get_x() + ((0.5 + x) * square_width)
        ),
        static_cast<int>(
          rect.get_tl().get_y() + ((0.5 + y) * square_height)
        )
      };
      s.setPosition(square_pos.get_x(), square_pos.get_y());
      window.draw(s);
    }
  }
}

void test_sfml_helper()
{
#ifndef NDEBUG
  // f_health_to_color
  {
    const sf::Color lowest{f_health_to_color(1.0 * 0.125)};
    const sf::Color low{f_health_to_color(3.0 * 0.125)};
    const sf::Color mid{f_health_to_color(5.0 * 0.125)};
    const sf::Color high{f_health_to_color(7.0 * 0.125)};
    assert(lowest != low);
    assert(lowest != mid);
    assert(lowest != high);
    assert(low != mid);
    assert(low != high);
    assert(mid != high);
  }
  // set_rect
  {
    sf::RectangleShape r;
    const screen_rect screen_size{get_default_screen_size()};
    set_rect(r, screen_size);
    assert(r.getSize().x == get_width(screen_size));
    assert(r.getSize().y == get_height(screen_size));
  }
  // set_text_position
  {
    sf::Text t;
    t.setString("X");
    const screen_rect screen_size{get_default_screen_size()};
    set_text_position(t, screen_size);
    assert(!t.getString().isEmpty()); // Does not test set_text_poistion at all
  }
  // to_str
  {
    assert(to_str(sf::Keyboard::Key::A) == "A");
    assert(to_str(sf::Keyboard::Key::Add) == "Add");
    assert(to_str(sf::Keyboard::Key::B) == "B");
    assert(to_str(sf::Keyboard::Key::Backslash) == "Backslash");
    assert(to_str(sf::Keyboard::Key::Backspace) == "Backspace");
    assert(to_str(sf::Keyboard::Key::C) == "C");
    assert(to_str(sf::Keyboard::Key::Comma) == "Comma");
    assert(to_str(sf::Keyboard::Key::D) == "D");
    assert(to_str(sf::Keyboard::Key::Delete) == "Delete");
    assert(to_str(sf::Keyboard::Key::Divide) == "Divide");
    assert(to_str(sf::Keyboard::Key::Down) == "Down");
    assert(to_str(sf::Keyboard::Key::E) == "E");
    assert(to_str(sf::Keyboard::Key::End) == "End");
    assert(to_str(sf::Keyboard::Key::Enter) == "Enter");
    assert(to_str(sf::Keyboard::Key::Equal) == "Equal");
    assert(to_str(sf::Keyboard::Key::Escape) == "Escape");
    assert(to_str(sf::Keyboard::Key::F10) == "F10");
    assert(to_str(sf::Keyboard::Key::F11) == "F11");
    assert(to_str(sf::Keyboard::Key::F12) == "F12");
    assert(to_str(sf::Keyboard::Key::F13) == "F13");
    assert(to_str(sf::Keyboard::Key::F14) == "F14");
    assert(to_str(sf::Keyboard::Key::F15) == "F15");
    assert(to_str(sf::Keyboard::Key::F1) == "F1");
    assert(to_str(sf::Keyboard::Key::F2) == "F2");
    assert(to_str(sf::Keyboard::Key::F3) == "F3");
    assert(to_str(sf::Keyboard::Key::F4) == "F4");
    assert(to_str(sf::Keyboard::Key::F5) == "F5");
    assert(to_str(sf::Keyboard::Key::F6) == "F6");
    assert(to_str(sf::Keyboard::Key::F7) == "F7");
    assert(to_str(sf::Keyboard::Key::F8) == "F8");
    assert(to_str(sf::Keyboard::Key::F9) == "F9");
    assert(to_str(sf::Keyboard::Key::F) == "F");
    assert(to_str(sf::Keyboard::Key::G) == "G");
    assert(to_str(sf::Keyboard::Key::H) == "H");
    assert(to_str(sf::Keyboard::Key::Home) == "Home");
    assert(to_str(sf::Keyboard::Key::Hyphen) == "Hyphen");
    assert(to_str(sf::Keyboard::Key::I) == "I");
    assert(to_str(sf::Keyboard::Key::Insert) == "Insert");
    assert(to_str(sf::Keyboard::Key::J) == "J");
    assert(to_str(sf::Keyboard::Key::K) == "K");
    assert(to_str(sf::Keyboard::Key::KeyCount) == "KeyCount");
    assert(to_str(sf::Keyboard::Key::L) == "L");
    assert(to_str(sf::Keyboard::Key::LAlt) == "LAlt");
    assert(to_str(sf::Keyboard::Key::LBracket) == "LBracket");
    assert(to_str(sf::Keyboard::Key::LControl) == "LControl");
    assert(to_str(sf::Keyboard::Key::LShift) == "LShift");
    assert(to_str(sf::Keyboard::Key::LSystem) == "LSystem");
    assert(to_str(sf::Keyboard::Key::Left) == "Left");
    assert(to_str(sf::Keyboard::Key::M) == "M");
    assert(to_str(sf::Keyboard::Key::Menu) == "Menu");
    assert(to_str(sf::Keyboard::Key::Multiply) == "Multiply");
    assert(to_str(sf::Keyboard::Key::N) == "N");
    assert(to_str(sf::Keyboard::Key::Num0) == "Num0");
    assert(to_str(sf::Keyboard::Key::Num1) == "Num1");
    assert(to_str(sf::Keyboard::Key::Num2) == "Num2");
    assert(to_str(sf::Keyboard::Key::Num3) == "Num3");
    assert(to_str(sf::Keyboard::Key::Num4) == "Num4");
    assert(to_str(sf::Keyboard::Key::Num5) == "Num5");
    assert(to_str(sf::Keyboard::Key::Num6) == "Num6");
    assert(to_str(sf::Keyboard::Key::Num7) == "Num7");
    assert(to_str(sf::Keyboard::Key::Num8) == "Num8");
    assert(to_str(sf::Keyboard::Key::Num9) == "Num9");
    assert(to_str(sf::Keyboard::Key::Numpad0) == "Numpad0");
    assert(to_str(sf::Keyboard::Key::Numpad1) == "Numpad1");
    assert(to_str(sf::Keyboard::Key::Numpad2) == "Numpad2");
    assert(to_str(sf::Keyboard::Key::Numpad3) == "Numpad3");
    assert(to_str(sf::Keyboard::Key::Numpad4) == "Numpad4");
    assert(to_str(sf::Keyboard::Key::Numpad5) == "Numpad5");
    assert(to_str(sf::Keyboard::Key::Numpad6) == "Numpad6");
    assert(to_str(sf::Keyboard::Key::Numpad7) == "Numpad7");
    assert(to_str(sf::Keyboard::Key::Numpad8) == "Numpad8");
    assert(to_str(sf::Keyboard::Key::Numpad9) == "Numpad9");
    assert(to_str(sf::Keyboard::Key::O) == "O");
    assert(to_str(sf::Keyboard::Key::P) == "P");
    assert(to_str(sf::Keyboard::Key::PageDown) == "PageDown");
    assert(to_str(sf::Keyboard::Key::PageUp) == "PageUp");
    assert(to_str(sf::Keyboard::Key::Pause) == "Pause");
    assert(to_str(sf::Keyboard::Key::Period) == "Period");
    assert(to_str(sf::Keyboard::Key::Q) == "Q");
    assert(to_str(sf::Keyboard::Key::Quote) == "Quote");
    assert(to_str(sf::Keyboard::Key::R) == "R");
    assert(to_str(sf::Keyboard::Key::RAlt) == "RAlt");
    assert(to_str(sf::Keyboard::Key::RBracket) == "RBracket");
    assert(to_str(sf::Keyboard::Key::RControl) == "RControl");
    assert(to_str(sf::Keyboard::Key::RShift) == "RShift");
    assert(to_str(sf::Keyboard::Key::RSystem) == "RSystem");
    assert(to_str(sf::Keyboard::Key::Right) == "Right");
    assert(to_str(sf::Keyboard::Key::S) == "S");
    assert(to_str(sf::Keyboard::Key::Semicolon) == "Semicolon");
    assert(to_str(sf::Keyboard::Key::Slash) == "Slash");
    assert(to_str(sf::Keyboard::Key::Space) == "Space");
    assert(to_str(sf::Keyboard::Key::Subtract) == "Subtract");
    assert(to_str(sf::Keyboard::Key::T) == "T");
    assert(to_str(sf::Keyboard::Key::Tab) == "Tab");
    assert(to_str(sf::Keyboard::Key::Tilde) == "Tilde");
    assert(to_str(sf::Keyboard::Key::U) == "U");
    assert(to_str(sf::Keyboard::Key::Unknown) == "Unknown");
    assert(to_str(sf::Keyboard::Key::Up) == "Up");
    assert(to_str(sf::Keyboard::Key::V) == "V");
    assert(to_str(sf::Keyboard::Key::W) == "W");
    assert(to_str(sf::Keyboard::Key::X) == "X");
    assert(to_str(sf::Keyboard::Key::Y) == "Y");
    assert(to_str(sf::Keyboard::Key::Z) == "Z");

  }
  // to_sfml_color
  {
    const sf::Color b{to_sfml_color(chess_color::black)};
    const sf::Color w{to_sfml_color(chess_color::white)};
    assert(b != w);
  }
  // to_sfml_color
  {
    const sf::Color ba{to_sfml_color(chess_color::black, piece_action_type::attack)};
    const sf::Color bm{to_sfml_color(chess_color::black, piece_action_type::move)};
    const sf::Color wa{to_sfml_color(chess_color::white, piece_action_type::attack)};
    const sf::Color wm{to_sfml_color(chess_color::white, piece_action_type::move)};
    assert(ba != bm);
    assert(ba != wa);
    assert(ba != wm);
    assert(bm != wa);
    assert(bm != wm);
    assert(wa != wm);
  }
#endif
}

sf::Color to_sfml_color(const chess_color color) noexcept
{
  if (color == chess_color::white) return sf::Color::White;
  assert(color == chess_color::black);
  return sf::Color::Black;
}

sf::Color to_sfml_color(
  const chess_color color,
  const piece_action_type t
) noexcept
{
  if (color == chess_color::white)
  {
    if (t == piece_action_type::move || t == piece_action_type::promote)
    {
      return sf::Color(255 - 64, 255 - 0, 255 - 64);
    }
    assert(t == piece_action_type::attack
      || t == piece_action_type::castle_kingside
      || t == piece_action_type::castle_queenside
    );
    return sf::Color(255 - 0, 255 - 64, 255 - 64);
  }
  assert(color == chess_color::black);
  if (t == piece_action_type::move || t == piece_action_type::promote)
  {
    return sf::Color(128 - 64, 128 - 0, 128 - 64);
  }
  assert(t == piece_action_type::attack
    || t == piece_action_type::castle_kingside
    || t == piece_action_type::castle_queenside
  );
  return sf::Color(128 - 0, 128 - 64, 128 - 64);
}

std::string to_str(const sf::Keyboard::Key k)
{
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
}
