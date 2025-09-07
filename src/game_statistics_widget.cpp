#include "game_statistics_widget.h"

#ifndef LOGIC_ONLY

#include "draw.h"
#include "game_resources.h"
#include "game_statistics_widget_layout.h"
#include "game_controller.h"
#include "game_statistics.h"

#include <cassert>

void draw_game_statistics_widget(
  const game_statistics_widget_layout& layout,
  const game_controller& c
)
{
  // Background
  draw_rectangle(layout.get_background(), sf::Color(128, 128, 128, 128));

  // Draw all symbols
  for (const auto s: get_all_game_statistic_types())
  {
    draw_texture(
      game_resources::get().get_board_game_textures().get_symbol(s),
      layout.get_symbol(s),
      game_resources::get().get_board_game_textures().get_symbol_color(s)
    );
  }

  // Clock time
  {
    const auto& r{layout.get_relative(game_statistic_type::time)};
    draw_rectangle(
      r,
      sf::Color(128, 128, 128, 128)
    );
    const std::string s{to_human_str(get_in_game_time(c))};
    const auto text_rect{create_centered_rect(get_center(r), 80, 48)};
    draw_text(s, text_rect, 32);
  }

  // Statistic
  const game_statistics statistics(c);

  for (const game_statistic_type statistic: get_all_game_statistic_types())
  {
    // Time is displayed differently
    if (statistic == game_statistic_type::time) continue;

    // Relative, the two bars against each other
    {
      const double f{statistics.calc_relative(statistic)};
      assert(f >= 0.0);
      assert(f <= 1.0);
      const auto& r_border{layout.get_relative(statistic)};
      const auto r{create_rect_inside(r_border)};
      for (const side s: get_all_sides())
      {
        try
        {
          const double f_side{s == side::lhs ? f : 1.0 - f};
          assert(f_side >= 0.0);
          assert(f_side <= 1.0);
          const screen_rect bar_rect{create_partial_rect_from_side(s, r, f_side)};
          const sf::Color bar_color{
            game_resources::get().get_board_game_textures().get_bar_color(
              statistic,
              c.get_lobby_options().get_color(s)
            )
          };
          draw_rectangle(bar_rect, bar_color);
        }
        catch (std::logic_error& e)
        {
          // OK
        }
      }
      draw_outline(
        r_border,
        game_resources::get().get_board_game_textures().get_outline_color(statistic),
        1
      );
    }
    // Absolute
    for (const side s: get_all_sides())
    {
      double f{statistics.get(statistic, s)};
      assert(f >= 0.0);
      if (statistic == game_statistic_type::value)
      {
        // Piece value can go beyond 1.0 if a pawn promotes to a queen
        if (f >= 1.0) f = 1.0;
      }
      assert(f <= 1.0);
      const auto& r_border{layout.get_absolute(statistic, s)};
      try
      {
        const auto r{create_rect_inside(r_border)};
        // Start the bar at the player's side
        const screen_rect bar_rect{create_partial_rect_from_side(s, r, f)};
        const sf::Color bar_color{
          game_resources::get().get_board_game_textures().get_bar_color(
            statistic,
            c.get_lobby_options().get_color(s)
          )
        };
        draw_rectangle(
          bar_rect,
          bar_color
        );
        draw_outline(
          bar_rect,
          game_resources::get().get_board_game_textures().get_outline_color(statistic),
          1
        );
      }
      catch (std::logic_error& e)
      {
        // OK
      }
      draw_outline(
        r_border,
        game_resources::get().get_board_game_textures().get_outline_color(statistic),
        1
      );
    }
  }
}

#endif // LOGIC_ONLY
