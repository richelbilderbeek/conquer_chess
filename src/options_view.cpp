#include "options_view.h"

#ifndef LOGIC_ONLY

#include "draw.h"
#include "draw_board.h"
#include "physical_controller.h"
#include "physical_controllers.h"
#include "physical_controller_type.h"
#include "game_controller.h"
#include "options_view.h"
#include "physical_controllers.h"
#include "game_resources.h"
#include "screen_coordinate.h"
#include "game_options.h"
//#include "pieces.h"
#include "physical_controllers.h"
#include "sfml_helper.h"
#include "options_view.h"
#include "render_window.h"

#include <cassert>

options_view::options_view()
  : m_selected{options_view_item::game_speed}
{
  m_controls_bar.set_draw_player_controls(false);
  m_controls_bar.set_draw_invert(true);
  m_controls_bar.set_draw_left_right_increase_descrease(true);
  m_controls_bar.set_draw_enter(true);
  m_controls_bar.set_escape_button_text("Reject changes and go back");

}

void options_view::decrease_selected()
{
  switch (m_selected)
  {
    case options_view_item::game_speed: decrease_game_speed(m_game_options); break;
    case options_view_item::music_volume: decrease_music_volume(m_game_options); break;
    case options_view_item::sound_effects_volume: decrease_sound_effects_volume(m_game_options); break;
    case options_view_item::starting_position: decrease_starting_position(m_game_options); break;
    case options_view_item::left_controls:
    {
      set_next_state(program_state::left_controls);
    }
    break;
    case options_view_item::right_controls:
    {
      set_next_state(program_state::right_controls);
    }
    break;
  }
  game_resources::get().get_sound_effects().play_hide();
}

void options_view::increase_selected()
{
  switch (m_selected)
  {
    case options_view_item::game_speed: increase_game_speed(m_game_options); break;
    case options_view_item::music_volume: increase_music_volume(m_game_options); break;
    case options_view_item::sound_effects_volume: increase_sound_effects_volume(m_game_options); break;
    case options_view_item::starting_position: increase_starting_position(m_game_options); break;
    case options_view_item::left_controls:
    {
      set_next_state(program_state::left_controls);
    }
    break;
    case options_view_item::right_controls:
    {
      set_next_state(program_state::right_controls);
    }
    break;
  }
  game_resources::get().get_sound_effects().play_hide();
}

void draw_panel(
  options_view& ,
  const screen_rect& panel_position,
  const std::string panel_text,
  const chess_color color
)
{
  draw_texture(
    get_strip_texture(color),
    panel_position
  );
  draw_normal_text(panel_text, panel_position);
}

bool options_view::process_event_impl(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    set_next_state(program_state::main_menu);
    return false;
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      set_next_state(program_state::main_menu);
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::Return)
    {
      m_has_accepted = true;
      set_next_state(program_state::main_menu);
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::G)
    {
      if (is_shift_pressed())
      {
        decrease_game_speed(m_game_options);
      }
      else
      {
        increase_game_speed(m_game_options);
      }
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::M)
    {
      if (is_shift_pressed())
      {
        decrease_music_volume(m_game_options);
      }
      else
      {
        increase_music_volume(m_game_options);
      }
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::Q)
    {
      set_next_state(program_state::main_menu);
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::S)
    {
      if (is_shift_pressed())
      {
        decrease_sound_effects_volume(m_game_options);
      }
      else
      {
        increase_sound_effects_volume(m_game_options);
      }
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::T)
    {
      if (is_shift_pressed())
      {
        decrease_starting_position(m_game_options);
      }
      else
      {
        increase_starting_position(m_game_options);
      }
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::Num1)
    {
      set_next_state(program_state::left_controls);
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::Num2)
    {
      set_next_state(program_state::right_controls);
      return false;
    }
    else if (key_pressed == sf::Keyboard::Key::Up)
    {
      set_selected(get_above(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Right)
    {
      switch (m_selected)
      {
        case options_view_item::game_speed:
        default:
        assert(m_selected == options_view_item::game_speed);
        case options_view_item::music_volume:
        case options_view_item::sound_effects_volume:
        case options_view_item::starting_position:
          increase_selected();
        break;
        case options_view_item::left_controls:
        case options_view_item::right_controls:
          set_selected(get_right_of(m_selected));
        break;
      }
    }
    else if (key_pressed == sf::Keyboard::Key::Down)
    {
      set_selected(get_below(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Left)
    {
      switch (m_selected)
      {
        case options_view_item::game_speed:
        default:
        assert(m_selected == options_view_item::game_speed);
        case options_view_item::music_volume:
        case options_view_item::sound_effects_volume:
        case options_view_item::starting_position:
          decrease_selected();
        break;
        case options_view_item::left_controls:
        case options_view_item::right_controls:
          set_selected(get_left_of(m_selected));
        break;
      }
    }
    else if (key_pressed == sf::Keyboard::Key::Space)
    {
      increase_selected();
    }
  }
  else if (event.type == sf::Event::MouseMoved)
  {
    const auto mouse_screen_pos{
      screen_coordinate(event.mouseMove.x, event.mouseMove.y)
    };
    if (is_in(mouse_screen_pos, m_layout.get_chess_board().get_board())) set_selected(options_view_item::starting_position);
    else if (is_in(mouse_screen_pos, m_layout.get_game_speed_value())) set_selected(options_view_item::game_speed);
    else if (is_in(mouse_screen_pos, m_layout.get_controller_type_value(side::lhs))) set_selected(options_view_item::left_controls);
    else if (is_in(mouse_screen_pos, m_layout.get_music_volume_value())) set_selected(options_view_item::music_volume);
    else if (is_in(mouse_screen_pos, m_layout.get_controller_type_value(side::rhs))) set_selected(options_view_item::right_controls);
    else if (is_in(mouse_screen_pos, m_layout.get_sound_effects_volume_value())) set_selected(options_view_item::sound_effects_volume);
    else if (is_in(mouse_screen_pos, m_layout.get_starting_pos_value())) set_selected(options_view_item::starting_position);
  }
  else if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Button::Left)
    {
      increase_selected();
    }
    else if (event.mouseButton.button == sf::Mouse::Button::Right)
    {
      decrease_selected();
    }
  }
  return false; // if no events proceed with tick
}


void options_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_rect w(
    screen_coordinate(0, 0),
    screen_coordinate(event.size.width, event.size.height)
  );
  m_layout = options_view_layout(
    w,
    get_default_margin_width()
  );
  m_controls_bar.set_screen_rect(w);
}

void options_view::set_selected(const options_view_item i)
{
  if (m_selected != i)
  {
    game_resources::get().get_sound_effects().play_hide();
  }
  m_selected = i;
}

void options_view::draw_impl()
{
  draw_background(*this);
  draw_layout_panels(*this);

  draw_top(*this);
  draw_bottom(*this);
  draw_selected_panel(*this);

  const bool show_semitransparent{true};
  draw_squares(
    m_layout.get_chess_board(),
    show_semitransparent
  );
  draw_pieces(*this);

  // Update the controls bar
  m_controls_bar.set_draw_left_right_increase_descrease(
       m_selected == options_view_item::game_speed
    || m_selected == options_view_item::music_volume
    || m_selected == options_view_item::sound_effects_volume
    || m_selected == options_view_item::starting_position
  );
  m_controls_bar.set_draw_select(
       m_selected == options_view_item::left_controls
    || m_selected == options_view_item::right_controls
  );
  m_controls_bar.draw();

}

void draw_background(options_view& v)
{
  draw_texture(
    game_resources::get().get_options_menu_textures().get_texture(options_view_item::game_speed),
    v.get_layout().get_background()
  );
}

void draw_bottom(options_view& v)
{
  draw_bottom_header(v);
  draw_bottom_row(v, side::lhs);
  draw_bottom_row(v, side::rhs);
}

void draw_bottom_header(options_view& v)
{
  const auto& layout{v.get_layout()};
  const auto& screen_rect = layout.get_controls_label();
  draw_controls_label(screen_rect);
}

void draw_bottom_row(options_view& v, const side player_side)
{
  const auto& layout{v.get_layout()};
  const auto sr{layout.get_controller_type_value(player_side)};
  const physical_controller_type t{v.get_physical_controllers().get_controller(player_side).get_type()};
  draw_fancy_physical_controller_label(t, sr);
  const sf::Keyboard::Key k{
    player_side == side::lhs ? sf::Keyboard::Key::Num1 : sf::Keyboard::Key::Num2
  };
  const int x1{sr.get_tl().get_x() + 16};
  const int x2{x1 + 64};
  const int y1{sr.get_tl().get_y() + 16};
  const int y2{y1 + 64};
  const screen_rect r{
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  };
  draw_input_prompt_symbol_on_background(k, r, sf::Color(128, 128, 128, 128));
}

void draw_game_speed(options_view& v)
{
  const auto& layout = v.get_layout();
  draw_game_speed_icon(layout.get_game_speed_label());
  draw_game_speed_value(layout.get_game_speed_value(), v.get_game_options().get_game_speed());
}


void draw_pieces(options_view& view)
{
  game_controller c(
    create_game_with_starting_position(get_starting_position(view.get_game_options()))
  );
  const bool indicate_protectedness{false};
  draw_pieces(
    c,
    view.get_layout().get_chess_board(),
    indicate_protectedness
  );
}

void draw_starting_position(options_view& v)
{
  const auto& layout = v.get_layout();
  draw_starting_position_label(layout.get_starting_pos_label());
  draw_starting_position_value(layout.get_starting_pos_value(), get_starting_position(v.get_game_options()));
}

void draw_top(options_view& v)
{
  assert(!to_str(get_starting_position(v.get_game_options())).empty());

  draw_game_speed(v);
  draw_music_volume(v);
  draw_sound_effects_volume(v);
  draw_starting_position(v);

  assert(!to_str(get_starting_position(v.get_game_options())).empty());
}


void draw_layout_panels(options_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(255, 255, 255, 128));
    get_render_window().draw(rectangle);
  }
}

void draw_music_volume(options_view& v)
{
  const auto& layout = v.get_layout();
  draw_music_volume_label(layout.get_music_volume_label());
  draw_music_volume_value(layout.get_music_volume_value(), v.get_game_options().get_music_volume());
}

void draw_selected_panel(options_view& v)
{
  draw_outline(v.get_layout().get_selectable_rect(v.get_selected()));
}

void draw_sound_effects_volume(options_view& v)
{
  const auto& layout = v.get_layout();
  draw_sound_effects_volume_label(layout.get_sound_effects_volume_label());
  draw_sound_effects_volume_value(layout.get_sound_effects_volume_value(), v.get_game_options().get_sound_effects_volume());
}

void options_view::start_impl()
{
  assert(!is_active());
  set_is_active(true);
}

void options_view::stop_impl()
{
  assert(is_active());
  clear_next_state();
  set_is_active(false);
}

void options_view::tick_impl(const delta_t)
{
  assert(is_active());
  // Nothing here yet
}


#endif // LOGIC_ONLY
