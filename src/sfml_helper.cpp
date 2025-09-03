#include "sfml_helper.h"

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>
#include <cmath>

std::string button_str_to_resource_name(std::string button_str)
{
  // This can be made constant-time, by using a LUT
  assert(!button_str.empty());

  if (button_str == "ButtonCount" || button_str == "XButton1" || button_str == "XButton2")
  {
    return "keyboard_question_outline";
  }
  if (button_str == "Middle") return "mouse_scroll";

  button_str[0] = std::tolower(button_str[0]);
  return std::string("mouse_") + button_str;
}

sf::Color f_health_to_color(const double f)
{
  assert(f >= 0.0);
  assert(f <= 1.0);
  if (f < 0.25) return sf::Color::Red;
  if (f < 0.50) return sf::Color(255, 128, 0);
  if (f < 0.75) return sf::Color::Yellow;
  return sf::Color::Green;
}

sf::Color f_shield_to_color(const double f)
{
  assert(f >= 0.0);
  assert(f <= 1.0);
  if (f < 0.25) return sf::Color(0, 0, 252);
  if (f < 0.50) return sf::Color(0, 0, 253);
  if (f < 0.75) return sf::Color(0, 0, 254);
  return sf::Color(0, 0, 255);
}

std::vector<sf::Mouse::Button> get_all_sfml_buttons() noexcept
{
  const auto a{magic_enum::enum_values<sf::Mouse::Button>()};
  std::vector<sf::Mouse::Button> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

std::vector<sf::Keyboard::Key> get_all_sfml_keys() noexcept
{
  const auto a{magic_enum::enum_values<sf::Keyboard::Key>()};
  std::vector<sf::Keyboard::Key> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

#ifndef LOGIC_ONLY
bool is_shift_pressed() noexcept
{
  return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)
    || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)
  ;
}
#endif // LOGIC_ONLY

std::string key_str_to_resource_name(std::string key_str)
{
  // This can be made constant-time, by using a LUT
  assert(!key_str.empty());
  if (key_str == "Unknown"
    || key_str == "F13"
    || key_str == "F14"
    || key_str == "F15"
    || key_str == "Pause"
    || key_str == "KeyCount"
  )
  {
    // Keys without a symbol
    return "keyboard_question_outline";
  }
  if (key_str == "LAlt" || key_str == "RAlt") return "keyboard_alt";
  if (key_str == "LControl" || key_str == "RControl") return "keyboard_ctrl";
  if (key_str == "LShift" || key_str == "RShift") return "keyboard_shift";
  if (key_str == "LSystem" || key_str == "RSystem") return "keyboard_win";
  if (key_str == "Menu") return "keyboard_function";
  if (key_str == "LBracket") return "keyboard_bracket_open";
  if (key_str == "RBracket") return "keyboard_bracket_close";
  if (key_str == "Slash" || key_str == "Divide") return "keyboard_slash_forward";
  if (key_str == "BackSlash" || key_str == "Backslash") return "keyboard_slash_back";
  if (key_str == "Grave" || key_str == "Hyphen") return "keyboard_apostrophe";
  if (key_str == "Equal") return "keyboard_equals";
  if (key_str == "PageUp") return "keyboard_page_up";
  if (key_str == "PageDown") return "keyboard_page_down";
  if (key_str == "Add") return "keyboard_plus";
  if (key_str == "Subtract") return "keyboard_minus";
  if (key_str == "Multiply") return "keyboard_asterisk";
  if (key_str == "Up") return "keyboard_arrow_up";
  if (key_str == "Right") return "keyboard_arrow_right";
  if (key_str == "Down") return "keyboard_arrow_down";
  if (key_str == "Left") return "keyboard_arrow_left";
  if (key_str.size() > 6 && key_str.substr(0, 6) == std::string("Numpad"))
  {
     key_str = key_str.substr(6, key_str.size() - 6);
  }
  else if (key_str.size() > 3 && key_str.substr(0, 3) == std::string("Num"))
  {
     key_str = key_str.substr(3, key_str.size() - 3);
  }

  key_str[0] = std::tolower(key_str[0]);


  return std::string("keyboard_") + key_str;
}

void set_rect(sf::RectangleShape& rectangle, const screen_coordinate& screen_size)
{
  set_rect(
    rectangle,
    screen_rect(
      screen_coordinate(0, 0),
      screen_size
    )
  );
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
  const double text_center_y{0.5 * static_cast<double>(text.getLocalBounds().height)};

  // Text always seems to appear too low, move it somewhat up
  const double f_correction{0.08}; // Set to zero for no correction
  const double corrected_text_center_y{
    text_center_y + (f_correction * get_height(screen_rect))
  };
  text.setOrigin(
    0.5 * text.getLocalBounds().width,
    corrected_text_center_y
  );

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
  // f_shield_to_color
  {
    const sf::Color lowest{f_shield_to_color(1.0 * 0.125)};
    const sf::Color low{f_shield_to_color(3.0 * 0.125)};
    const sf::Color mid{f_shield_to_color(5.0 * 0.125)};
    const sf::Color high{f_shield_to_color(7.0 * 0.125)};
    assert(lowest != low);
    assert(lowest != mid);
    assert(lowest != high);
    assert(low != mid);
    assert(low != high);
    assert(mid != high);
  }

  // get_all_sfml_buttons
  {
    const auto v{get_all_sfml_buttons()};
    assert(!v.empty());
  }

  // get_all_sfml_keys
  {
    const auto v{get_all_sfml_keys()};
    assert(!v.empty());
  }

  #ifndef LOGIC_ONLY
  // is_shift_pressed
  {
    const auto b{is_shift_pressed()};
    assert(b || !b);
  }
  #endif // LOGIC_ONLY

  // key_str_to_resource_name
  {
    assert(key_str_to_resource_name("A") == "keyboard_a");
    assert(key_str_to_resource_name("Unknown") == "keyboard_question_outline");
    assert(key_str_to_resource_name("Num0") == "keyboard_0");
    assert(key_str_to_resource_name("LAlt") == "keyboard_alt");
    assert(key_str_to_resource_name("RAlt") == "keyboard_alt");
    assert(key_str_to_resource_name("LControl") == "keyboard_ctrl");
    assert(key_str_to_resource_name("RControl") == "keyboard_ctrl");
    assert(key_str_to_resource_name("LShift") == "keyboard_shift");
    assert(key_str_to_resource_name("RShift") == "keyboard_shift");
    assert(key_str_to_resource_name("LSystem") == "keyboard_win");
    assert(key_str_to_resource_name("RSystem") == "keyboard_win");
  }

  // set_rect, on screen_rect
  {
    sf::RectangleShape r;
    const screen_rect screen_size{get_default_screen_rect()};
    set_rect(r, screen_size);
    assert(r.getSize().x == get_width(screen_size));
    assert(r.getSize().y == get_height(screen_size));
  }

  // set_rect, on screen_coordinate
  {
    sf::RectangleShape r;
    const screen_coordinate screen_size(320, 200);
    set_rect(r, screen_size);
    assert(r.getSize().x == screen_size.get_x());
    assert(r.getSize().y == screen_size.get_y());
  }

  // set_text_position
  {
    sf::Text t;
    t.setString("X");
    const screen_rect screen_size{get_default_screen_rect()};
    set_text_position(t, screen_size);
    assert(!t.getString().isEmpty()); // Does not test set_text_poistion at all
  }

  // to_resource_name, for keys
  {
    assert(to_resource_name(sf::Keyboard::Key::A) == "keyboard_a");
    assert(to_resource_name(sf::Keyboard::Key::Unknown) == "keyboard_question_outline");
    assert(to_resource_name(sf::Keyboard::Key::Num0) == "keyboard_0");
    assert(to_resource_name(sf::Keyboard::Key::Numpad0) == "keyboard_0");
    assert(to_resource_name(sf::Keyboard::Key::LAlt) == "keyboard_alt");
    assert(to_resource_name(sf::Keyboard::Key::RAlt) == "keyboard_alt");
    assert(to_resource_name(sf::Keyboard::Key::LControl) == "keyboard_ctrl");
    assert(to_resource_name(sf::Keyboard::Key::RControl) == "keyboard_ctrl");
    assert(to_resource_name(sf::Keyboard::Key::LShift) == "keyboard_shift");
    assert(to_resource_name(sf::Keyboard::Key::RShift) == "keyboard_shift");
    assert(to_resource_name(sf::Keyboard::Key::LSystem) == "keyboard_win");
    assert(to_resource_name(sf::Keyboard::Key::RSystem) == "keyboard_win");
    assert(to_resource_name(sf::Keyboard::Key::Menu) == "keyboard_function");
    assert(to_resource_name(sf::Keyboard::Key::LBracket) == "keyboard_bracket_open");
    assert(to_resource_name(sf::Keyboard::Key::RBracket) == "keyboard_bracket_close");
    assert(to_resource_name(sf::Keyboard::Key::Slash) == "keyboard_slash_forward");
    assert(to_resource_name(sf::Keyboard::Key::Grave) == "keyboard_apostrophe");
    assert(to_resource_name(sf::Keyboard::Key::Hyphen) == "keyboard_apostrophe");
    assert(to_resource_name(sf::Keyboard::Key::Equal) == "keyboard_equals");
    assert(to_resource_name(sf::Keyboard::Key::PageUp) == "keyboard_page_up");
    assert(to_resource_name(sf::Keyboard::Key::PageDown) == "keyboard_page_down");
    assert(to_resource_name(sf::Keyboard::Key::Add) == "keyboard_plus");
    assert(to_resource_name(sf::Keyboard::Key::Subtract) == "keyboard_minus");
    assert(to_resource_name(sf::Keyboard::Key::Multiply) == "keyboard_asterisk");
    assert(to_resource_name(sf::Keyboard::Key::Divide) == "keyboard_slash_forward");
    assert(to_resource_name(sf::Keyboard::Key::Up) == "keyboard_arrow_up");
    assert(to_resource_name(sf::Keyboard::Key::Right) == "keyboard_arrow_right");
    assert(to_resource_name(sf::Keyboard::Key::Down) == "keyboard_arrow_down");
    assert(to_resource_name(sf::Keyboard::Key::Left) == "keyboard_arrow_left");
    assert(to_resource_name(sf::Keyboard::Key::F13) == "keyboard_question_outline");
    assert(to_resource_name(sf::Keyboard::Key::F14) == "keyboard_question_outline");
    assert(to_resource_name(sf::Keyboard::Key::F15) == "keyboard_question_outline");
    assert(to_resource_name(sf::Keyboard::Key::Pause) == "keyboard_question_outline");
    assert(to_resource_name(sf::Keyboard::Key::KeyCount) == "keyboard_question_outline");
    {
      const auto x{"keyboard_slash_back"};
      const auto r{to_resource_name(sf::Keyboard::Key::BackSlash)};
      assert(x == r);
    }
    assert(to_resource_name(sf::Keyboard::Key::BackSlash) == "keyboard_slash_back");
  }

  // to_inverted_resource_name, for keys
  {
    assert(to_inverted_resource_name(sf::Keyboard::Key::A) == "keyboard_a_outline");
    assert(to_inverted_resource_name(sf::Keyboard::Key::Unknown) == "keyboard_question");
  }

  // to_resource_name, for mouse buttons
  {
    assert(to_resource_name(sf::Mouse::Button::Left) == "mouse_left");
    assert(to_resource_name(sf::Mouse::Button::Right) == "mouse_right");
    assert(to_resource_name(sf::Mouse::Button::Middle) == "mouse_scroll");
    assert(to_resource_name(sf::Mouse::Button::ButtonCount) == "keyboard_question_outline");
    assert(to_resource_name(sf::Mouse::Button::XButton1) == "keyboard_question_outline");
    assert(to_resource_name(sf::Mouse::Button::XButton2) == "keyboard_question_outline");
  }

  // to_str, for keys
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
    // Quote is a deprecated value
    {
      const std::string expected{"Apostrophe"};
      const std::string created{to_str(sf::Keyboard::Key::Quote)};
      assert(expected == created);
    }
    assert(to_str(sf::Keyboard::Key::Quote) == "Apostrophe"); // deprecated value
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
    // Tilde is a deprecated value
    {
      const std::string expected{"Grave"};
      const std::string created{to_str(sf::Keyboard::Key::Tilde)};
      assert(expected == created);
    }
    assert(to_str(sf::Keyboard::Key::Tilde) == "Grave"); // Deprecated value
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
  // to_sfml_color, all must be valid
  {
    for (const auto c: get_all_chess_colors())
    {
      for (const auto t: get_all_piece_action_types())
      {
        assert(to_sfml_color(c, t).r >= 0);
      }
    }
  }

  // 62: to_one_char_str
  {
    assert(to_one_char_str(sf::Keyboard::A) == "A");
    assert(to_one_char_str(sf::Keyboard::Num0) == "0");
    assert(to_one_char_str(sf::Keyboard::Num1) == "1");
    assert(to_one_char_str(sf::Keyboard::Num2) == "2");
    assert(to_one_char_str(sf::Keyboard::Num3) == "3");
    assert(to_one_char_str(sf::Keyboard::Num4) == "4");
    assert(to_one_char_str(sf::Keyboard::Num5) == "5");
    assert(to_one_char_str(sf::Keyboard::Num6) == "6");
    assert(to_one_char_str(sf::Keyboard::Num7) == "7");
    assert(to_one_char_str(sf::Keyboard::Num8) == "8");
    assert(to_one_char_str(sf::Keyboard::Num9) == "9");
    assert(to_one_char_str(sf::Keyboard::LBracket) == "[");
    assert(to_one_char_str(sf::Keyboard::RBracket) == "]");
    assert(to_one_char_str(sf::Keyboard::Semicolon) == ";");
    assert(to_one_char_str(sf::Keyboard::Comma) == ",");
    assert(to_one_char_str(sf::Keyboard::Period) == "'");
    assert(to_one_char_str(sf::Keyboard::Quote) == "\"");
    assert(to_one_char_str(sf::Keyboard::Slash) == "/");
    assert(to_one_char_str(sf::Keyboard::Backslash) == "\\");
    assert(to_one_char_str(sf::Keyboard::Tilde) == "~");
    assert(to_one_char_str(sf::Keyboard::Equal) == "=");
    assert(to_one_char_str(sf::Keyboard::Hyphen) == "-");
    assert(to_one_char_str(sf::Keyboard::Add) == "+");
    assert(to_one_char_str(sf::Keyboard::Subtract) == "-");
    assert(to_one_char_str(sf::Keyboard::Multiply) == "*");
    assert(to_one_char_str(sf::Keyboard::Divide) == "/");
    assert(to_one_char_str(sf::Keyboard::Numpad0) == "0");
    assert(to_one_char_str(sf::Keyboard::Numpad1) == "1");
    assert(to_one_char_str(sf::Keyboard::Numpad2) == "2");
    assert(to_one_char_str(sf::Keyboard::Numpad3) == "3");
    assert(to_one_char_str(sf::Keyboard::Numpad4) == "4");
    assert(to_one_char_str(sf::Keyboard::Numpad5) == "5");
    assert(to_one_char_str(sf::Keyboard::Numpad6) == "6");
    assert(to_one_char_str(sf::Keyboard::Numpad7) == "7");
    assert(to_one_char_str(sf::Keyboard::Numpad8) == "8");
    assert(to_one_char_str(sf::Keyboard::Numpad9) == "9");
  }
#endif // NDEBUG
}

std::string to_one_char_str(const sf::Keyboard::Key k)
{
  switch (k)
  {
    case sf::Keyboard::Num0: return "0";
    case sf::Keyboard::Num1: return "1";
    case sf::Keyboard::Num2: return "2";
    case sf::Keyboard::Num3: return "3";
    case sf::Keyboard::Num4: return "4";
    case sf::Keyboard::Num5: return "5";
    case sf::Keyboard::Num6: return "6";
    case sf::Keyboard::Num7: return "7";
    case sf::Keyboard::Num8: return "8";
    case sf::Keyboard::Num9: return "9";
    case sf::Keyboard::LBracket: return "[";
    case sf::Keyboard::RBracket: return "]";
    case sf::Keyboard::Semicolon: return ";";
    case sf::Keyboard::Comma: return ",";
    case sf::Keyboard::Period: return "'";
    case sf::Keyboard::Quote: return "\"";
    case sf::Keyboard::Slash: return "/";
    case sf::Keyboard::Backslash: return "\\";
    case sf::Keyboard::Tilde: return "~";
    case sf::Keyboard::Equal: return "=";
    case sf::Keyboard::Hyphen: return "-";
    case sf::Keyboard::Add: return "+";
    case sf::Keyboard::Subtract: return "-";
    case sf::Keyboard::Multiply: return "*";
    case sf::Keyboard::Divide: return "/";
    case sf::Keyboard::Numpad0: return "0";
    case sf::Keyboard::Numpad1: return "1";
    case sf::Keyboard::Numpad2: return "2";
    case sf::Keyboard::Numpad3: return "3";
    case sf::Keyboard::Numpad4: return "4";
    case sf::Keyboard::Numpad5: return "5";
    case sf::Keyboard::Numpad6: return "6";
    case sf::Keyboard::Numpad7: return "7";
    case sf::Keyboard::Numpad8: return "8";
    case sf::Keyboard::Numpad9: return "9";
    default:
      std::string s{to_str(k)};
      assert(s.length() == 1);
      return s;
  }
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
  if (t == piece_action_type::select || t == piece_action_type::unselect)
  {
    return sf::Color::Transparent;
  }
  if (color == chess_color::white)
  {
    if (t == piece_action_type::move
      || t == piece_action_type::promote_to_knight
      || t == piece_action_type::promote_to_bishop
      || t == piece_action_type::promote_to_rook
      || t == piece_action_type::promote_to_queen
    )
    {
      return sf::Color(255 - 64, 255 - 0, 255 - 64);
    }
    assert(t == piece_action_type::attack
      || t == piece_action_type::attack_en_passant
      || t == piece_action_type::castle_kingside
      || t == piece_action_type::castle_queenside
    );
    return sf::Color(255 - 0, 255 - 64, 255 - 64);
  }
  assert(color == chess_color::black);
  if (t == piece_action_type::move
    || t == piece_action_type::promote_to_knight
    || t == piece_action_type::promote_to_bishop
    || t == piece_action_type::promote_to_rook
    || t == piece_action_type::promote_to_queen
  )
  {
    return sf::Color(128 - 64, 128 - 0, 128 - 64);
  }
  assert(t == piece_action_type::attack
    || t == piece_action_type::attack_en_passant
    || t == piece_action_type::castle_kingside
    || t == piece_action_type::castle_queenside
  );
  return sf::Color(128 - 0, 128 - 64, 128 - 64);
}

std::string to_inverted_resource_name(const sf::Keyboard::Key k)
{
  std::string s{key_str_to_resource_name(to_str(k)) + "_outline"};
  const std::string to_remove{"_outline_outline"};
  if (s.size() > to_remove.size()
    && s.substr(s.size() - to_remove.size(), to_remove.size()) == to_remove
  )
  {
    return s.substr(0, s.size() - to_remove.size());
  }
  return s;
}

std::string to_resource_name(const sf::Keyboard::Key k)
{
  return key_str_to_resource_name(to_str(k));
}


std::string to_resource_name(const sf::Mouse::Button k)
{
  return button_str_to_resource_name(to_str(k));
}

std::string to_str(const sf::Keyboard::Key k)
{
  return std::string(magic_enum::enum_name(k));
}

std::string to_str(const sf::Mouse::Button k)
{
  return std::string(magic_enum::enum_name(k));
}
