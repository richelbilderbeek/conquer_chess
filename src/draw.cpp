#include "draw.h"

#ifndef LOGIC_ONLY

#include "board_layout.h"
//#include "game_coordinate.h"
#include "game_options.h"
#include "square_layout.h"
#include "game_resources.h"
#include "physical_controllers.h"
#include "piece.h"
#include "render_window.h"
#include "sfml_helper.h"
#include "navigation_controls_layout.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>
#include <sstream>

void draw_about_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_big_text(sf::String("About"), sr);
  const screen_rect key_rect{
    screen_coordinate(
      sr.get_tl().get_x() + 16,
      sr.get_tl().get_y() + 16
    ),
    screen_coordinate(
      sr.get_tl().get_x() + 16 + 64,
      sr.get_tl().get_y() + 16 + 64
    )
  };
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::A,
    key_rect,
    sf::Color(128, 128, 128, 128)
  );
  const screen_rect discord_rect{
    screen_coordinate(
      sr.get_br().get_x() - 64 - 16,
      sr.get_br().get_y() - 64 - 16
    ),
    screen_coordinate(
      sr.get_br().get_x() - 16,
      sr.get_br().get_y() - 16
    )
  };
  draw_discord_logo(
    discord_rect
  );
}

void draw_big_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 48);
}

void draw_chessboard_strip_texture(const chess_color c, const screen_rect& sr)
{
  draw_texture(
    get_strip_texture(c),
    sr
  );
}

void draw_controls_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;

  set_rect(rectangle, sr);
  rectangle.setTexture(
    &get_game_controls_icon(
      options_controls_item::left_controls
    )
  );
  // Zoom in
  rectangle.setTextureRect(
    sf::IntRect(
      0,
      rectangle.getTexture()->getSize().y / 4,
      rectangle.getTexture()->getSize().x,
      rectangle.getTexture()->getSize().y / 2
    )
  );
  get_render_window().draw(rectangle);

  draw_normal_fancy_text("Controls", sr);
}

void draw_discord_logo(const screen_rect& sr)
{
  sf::RectangleShape rectangle;

  set_rect(rectangle, sr);
  rectangle.setTexture(
    &game_resources::get().get_misc_textures().get_discord_logo()
  );
  get_render_window().draw(rectangle);
}

void draw_fancy_physical_controller_label(const physical_controller_type& t, const screen_rect& sr)
{
  draw_fancy_physical_controller_type(t, sr);
  draw_normal_fancy_text(to_str(t), sr);
}

void draw_fancy_physical_controller_type(const physical_controller_type& t, const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(
    &game_resources::get().get_physical_controller_textures().get_fancy(t)
  );
  // Zoom in
  rectangle.setTextureRect(
   sf::IntRect(
     0,
     rectangle.getTexture()->getSize().y / 4,
     rectangle.getTexture()->getSize().x,
     rectangle.getTexture()->getSize().y / 2
   )
  );
  get_render_window().draw(rectangle);
}

void draw_fancy_text(
  const sf::String& s,
  const screen_rect& sr,
  const int outer_character_size,
  const int inner_character_size
)
{
  assert(inner_character_size < outer_character_size);
  draw_text(s, sr, outer_character_size, sf::Color::Black);
  draw_text(s, sr, inner_character_size, sf::Color::White);
}

void draw_game_speed_icon(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(
    &get_game_controls_icon(
      options_controls_item::game_speed
      )
    );
  // Zoom in
  rectangle.setTextureRect(
    sf::IntRect(
      0,
      rectangle.getTexture()->getSize().y / 4,
      rectangle.getTexture()->getSize().x,
      rectangle.getTexture()->getSize().y / 2
      )
    );
  get_render_window().draw(rectangle);

  draw_normal_fancy_text("Game speed", sr);
  const sf::Keyboard::Key k{sf::Keyboard::Key::G};
  const int x1{sr.get_tl().get_x() + 16};
  const int x2{x1 + 64};
  const int y1{sr.get_tl().get_y() + 16};
  const int y2{y1 + 64};
  const screen_rect r{
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  };
  if (is_shift_pressed())
  {
    draw_input_prompt_inverted_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
  else
  {
    draw_input_prompt_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
}

void draw_when_to_make_a_move_icon(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::when_to_make_a_move
      )
    );
  // Zoom in
  rectangle.setTextureRect(
    sf::IntRect(
      0,
      rectangle.getTexture()->getSize().y / 4,
      rectangle.getTexture()->getSize().x,
      rectangle.getTexture()->getSize().y / 2
      )
    );
  get_render_window().draw(rectangle);

  draw_normal_fancy_text("When to make a move", sr);
  const sf::Keyboard::Key k{sf::Keyboard::Key::W};
  const int x1{sr.get_tl().get_x() + 16};
  const int x2{x1 + 64};
  const int y1{sr.get_tl().get_y() + 16};
  const int y2{y1 + 64};
  const screen_rect r{
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  };
  if (is_shift_pressed())
  {
    draw_input_prompt_inverted_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
  else
  {
    draw_input_prompt_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
}

void draw_game_speed_value(
  const screen_rect& sr,
  const game_speed speed
)
{
  draw_chessboard_strip_texture(chess_color::black, sr);
  draw_normal_text(to_human_str(speed), sr);
}

void draw_when_to_make_a_move_value(
  const screen_rect& sr,
  const when_to_make_a_move_law law
)
{
  draw_chessboard_strip_texture(chess_color::black, sr);
  draw_normal_text(to_human_str(law), sr);
}


void draw_huge_fancy_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 512, sf::Color::Black);
  draw_text(s, sr, 500, sf::Color::White);
}

void draw_input_prompt_inverted_symbol(
  const sf::Keyboard::Key& k,
  const screen_rect& r
)
{
  draw_texture(
    game_resources::get().get_input_prompt_textures().get_inverted_texture(k),
    r
  );
}

void draw_input_prompt_symbol(
  const sf::Keyboard::Key& k,
  const screen_rect& r
)
{
  draw_texture(
    game_resources::get().get_input_prompt_textures().get_texture(k),
    r
  );
}

void draw_input_prompt_symbol(
  const sf::Mouse::Button& k,
  const screen_rect& r
)
{
  draw_texture(
    game_resources::get().get_input_prompt_textures().get_texture(k),
    r
  );
}

void draw_input_prompt_inverted_symbol_on_background(
  const sf::Keyboard::Key& k,
  const screen_rect& r,
  const sf::Color background_color
)
{
  draw_rectangle(r, background_color);
  draw_input_prompt_inverted_symbol(k, r);
}

void draw_input_prompt_symbol_on_background(
  const sf::Keyboard::Key& k,
  const screen_rect& r,
  const sf::Color background_color
)
{
  draw_rectangle(r, background_color);
  draw_input_prompt_symbol(k, r);
}

void draw_music_volume_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::music_volume
      )
    );
  set_rect(rectangle, sr);
  // Zoom in
  rectangle.setTextureRect(
    sf::IntRect(
      0,
      rectangle.getTexture()->getSize().y / 4,
      rectangle.getTexture()->getSize().x,
      rectangle.getTexture()->getSize().y / 2
      )
    );
  get_render_window().draw(rectangle);

  draw_normal_fancy_text("Music volume", sr);
  const sf::Keyboard::Key k{sf::Keyboard::Key::M};
  const int x1{sr.get_tl().get_x() + 16};
  const int x2{x1 + 64};
  const int y1{sr.get_tl().get_y() + 16};
  const int y2{y1 + 64};
  const screen_rect r{
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  };
  if (is_shift_pressed())
  {
    draw_input_prompt_inverted_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
  else
  {
    draw_input_prompt_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
}

void draw_music_volume_value(
  const screen_rect& sr,
  const volume v
)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  std::stringstream s;
  s << v << " %";
  draw_normal_text(s.str(), sr);
}

void draw_navigation_controls(
  const physical_controllers& pc,
  const navigation_controls_layout& layout,
  const side p
)
{
  const auto& c{pc.get_controller(p)};
  switch (c.get_type())
  {
    case physical_controller_type::keyboard:
    {
      draw_input_prompt_symbol(
        c.get_key_bindings().get_key_for_move_up(),
        layout.get_up()
      );
      draw_input_prompt_symbol(
        c.get_key_bindings().get_key_for_move_left(),
        layout.get_left()
      );
      draw_input_prompt_symbol(
        c.get_key_bindings().get_key_for_move_down(),
        layout.get_down()
      );
      draw_input_prompt_symbol(
        c.get_key_bindings().get_key_for_move_right(),
        layout.get_right()
      );
    }
    break;
    case physical_controller_type::mouse:
    {
      draw_texture(
        game_resources::get().get_input_prompt_textures().get_texture("mouse_move"),
        layout.get_mouse()
      );
    }
    break;
  }
}



void draw_normal_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 32);
}

void draw_normal_fancy_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 32, sf::Color::Black);
  draw_text(s, sr, 30, sf::Color::White);
}

void draw_normal_texts(const std::vector<sf::String>& s, const screen_rect& r)
{
  draw_texts(s, r, 32);
}

void draw_options_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_big_text(sf::String("Options"), sr);
  const screen_rect key_rect{
    screen_coordinate(
      sr.get_tl().get_x() + 16,
      sr.get_tl().get_y() + 16
    ),
    screen_coordinate(
      sr.get_tl().get_x() + 16 + 64,
      sr.get_tl().get_y() + 16 + 64
    )
  };
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::O,
    key_rect,
    sf::Color(128, 128, 128, 128)
  );
}

void draw_options_controls_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_big_text(sf::String("Controls"), sr);
  const screen_rect key_rect{
    screen_coordinate(
      sr.get_tl().get_x() + 16,
      sr.get_tl().get_y() + 16
    ),
    screen_coordinate(
      sr.get_tl().get_x() + 16 + 64,
      sr.get_tl().get_y() + 16 + 64
    )
  };
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::C,
    key_rect,
    sf::Color(128, 128, 128, 128)
  );
}

void draw_options_laws_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_big_text(sf::String("Laws"), sr);
  const screen_rect key_rect{
    screen_coordinate(
      sr.get_tl().get_x() + 16,
      sr.get_tl().get_y() + 16
    ),
    screen_coordinate(
      sr.get_tl().get_x() + 16 + 64,
      sr.get_tl().get_y() + 16 + 64
    )
  };
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::L,
    key_rect,
    sf::Color(128, 128, 128, 128)
  );
}

void draw_options_video_and_audio_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_big_text(sf::String("Video and audio"), sr);
  const screen_rect key_rect{
    screen_coordinate(
      sr.get_tl().get_x() + 16,
      sr.get_tl().get_y() + 16
    ),
    screen_coordinate(
      sr.get_tl().get_x() + 16 + 64,
      sr.get_tl().get_y() + 16 + 64
    )
  };
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::V,
    key_rect,
    sf::Color(128, 128, 128, 128)
  );
}

void draw_outline(
  const screen_rect& sr,
  const sf::Color& outline_color,
  const int outline_thickness
)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(outline_color);
  rectangle.setOutlineThickness(outline_thickness);
  get_render_window().draw(rectangle);
}

void draw_physical_controller_symbol(const physical_controller_type& t, const screen_rect& sr)
{
  draw_texture(
    game_resources::get().get_physical_controller_textures().get_symbol(t),
    sr
  );
}



void draw_quit_button(const screen_rect& sr)
{
  assert(get_height(sr) == 100);
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_big_text(sf::String("Quit"), sr);
  const int y1{sr.get_tl().get_y() + 16};
  const int y2{y1 + 64};
  const int x1{sr.get_tl().get_x() + 16};
  const int x2{x1 + 64};
  const int x3{x2 + 0};
  const int x4{x3 + 64};
  const screen_rect key_rect_1{
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  };
  const screen_rect key_rect_2{
    screen_coordinate(x3, y1),
    screen_coordinate(x4, y2)
  };
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::Q,
    key_rect_1,
    sf::Color(128, 128, 128, 128)
  );
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::Escape,
    key_rect_2,
    sf::Color(128, 128, 128, 128)
  );

}

void draw_rectangle(const screen_rect& sr, const sf::Color& fill_color)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setFillColor(fill_color);
  get_render_window().draw(rectangle);
}

void draw_sound_effects_volume_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::sound_effects_volume
    )
  );
  // Zoom in
  rectangle.setTextureRect(
    sf::IntRect(
      0,
      rectangle.getTexture()->getSize().y / 4,
      rectangle.getTexture()->getSize().x,
      rectangle.getTexture()->getSize().y / 2
    )
  );
  set_rect(rectangle, sr);
  get_render_window().draw(rectangle);

  draw_normal_fancy_text("Sound effects volume", sr);
  const sf::Keyboard::Key k{sf::Keyboard::Key::S};
  const int x1{sr.get_tl().get_x() + 16};
  const int x2{x1 + 64};
  const int y1{sr.get_tl().get_y() + 16};
  const int y2{y1 + 64};
  const screen_rect r{
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  };
  if (is_shift_pressed())
  {
    draw_input_prompt_inverted_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
  else
  {
    draw_input_prompt_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
}

void draw_sound_effects_volume_value(
  const screen_rect& sr,
  const volume v
)
{
  draw_chessboard_strip_texture(chess_color::black, sr);

  std::stringstream s;
  s << v << " %";
  draw_normal_text(s.str(), sr);
}

void draw_start_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_big_text(sf::String("Start"), sr);
  const screen_rect key_rect{
    screen_coordinate(
      sr.get_tl().get_x() + 16,
      sr.get_tl().get_y() + 16
    ),
    screen_coordinate(
      sr.get_tl().get_x() + 16 + 64,
      sr.get_tl().get_y() + 16 + 64
    )
  };
  draw_input_prompt_symbol_on_background(
    sf::Keyboard::Key::S,
    key_rect,
    sf::Color(128, 128, 128, 128)
  );
}

void draw_starting_position_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::starting_position
    )
  );
  set_rect(rectangle, sr);
  // Zoom in
  rectangle.setTextureRect(
    sf::IntRect(
      0,
      rectangle.getTexture()->getSize().y / 4,
      rectangle.getTexture()->getSize().x,
      rectangle.getTexture()->getSize().y / 2
    )
  );
  get_render_window().draw(rectangle);

  draw_normal_fancy_text("Starting position", sr);
  const sf::Keyboard::Key k{sf::Keyboard::Key::T};
  const int x1{sr.get_tl().get_x() + 16};
  const int x2{x1 + 64};
  const int y1{sr.get_tl().get_y() + 16};
  const int y2{y1 + 64};
  const screen_rect r{
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  };
  if (is_shift_pressed())
  {
    draw_input_prompt_inverted_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
  else
  {
    draw_input_prompt_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
  }
}

void draw_starting_position_value(
  const screen_rect& sr,
  const starting_position_type t
)
{
  draw_texture(
    get_strip_texture(chess_color::white),
    sr
  );
  draw_normal_text(to_human_str(t), sr);
}

void draw_text(
  const sf::String& s,
  const screen_rect& sr,
  const int character_size,
  const sf::Color fill_color
)
{
  assert(character_size > 7);
  sf::Text text;
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setString(s);
  text.setCharacterSize(character_size);
  text.setFillColor(fill_color);
  set_text_position(text, sr);
  get_render_window().draw(text);
}

void draw_texts(
  const std::vector<sf::String>& s,
  const screen_rect& r,
  const int character_size,
  const sf::Color fill_color
  )
{
  const int n_lines = s.size();
  const double row_height{
    static_cast<double>(get_height(r))
    / static_cast<double>(s.size())
  };

  for (int i{0}; i!=n_lines; ++i)
  {
    const int x1{r.get_tl().get_x()};
    const int x2{r.get_br().get_x()};
    const int y1 = r.get_tl().get_y() + (row_height * i);
    const int y2 = r.get_tl().get_y() + (row_height * (i + 1));
    const screen_rect row_rect(
      screen_coordinate(x1, y1),
      screen_coordinate(x2, y2)
    );
    draw_text(s[i], row_rect, character_size, fill_color);
  }

}

void draw_texture(sf::Texture& t, const screen_rect& sr, const sf::Color& fill_color)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(&t);
  if (fill_color != sf::Color::Transparent)
  {
    rectangle.setFillColor(fill_color);
  }
  get_render_window().draw(rectangle);
}

void draw_texture(sf::Texture& t, const screen_coordinate& c, const screen_coordinate& size)
{
  const int w = size.get_x();
  const int h = size.get_y();
  const screen_rect sr{
    screen_coordinate(c.get_x() - (w / 2), c.get_y() - (h / 2)),
    screen_coordinate(c.get_x() + (w / 2), c.get_y() + (h / 2))
  };
  draw_texture(t, sr);
}

#endif // LOGIC_ONLY

