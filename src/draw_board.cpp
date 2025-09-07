#include "draw_board.h"

#ifndef LOGIC_ONLY

#include "board_layout.h"
#include "draw.h"
#include "game.h"
#include "helper.h"
#include "game_controller.h"
#include "game_resources.h"
#include "screen_coordinate.h"
#include "sfml_helper.h"
#include "render_window.h"

#include <cassert>
#include <iterator>

void draw_pieces(
  const game_controller& c,
  const board_layout& layout,
  const bool indicate_protectedness
)
{
  const auto& game{c.get_game()};

  const auto selected_piece_ids{collect_selected_piece_ids(c)};

  for (const auto& piece: game.get_pieces())
  {
    const int x{piece.get_current_square().get_x()};
    const int y{piece.get_current_square().get_y()};
    const square_layout& square_layout(layout.get_square(x, y));

    // Transparency effect when moving
    sf::Color fill_color = sf::Color::Transparent;
    if (!piece.get_actions().empty()
      && piece.get_actions()[0].get_action_type() == piece_action_type::move
    )
    {
      const double f{piece.get_current_action_progress().get()};
      int alpha{0};
      if (f < 0.5)
      {
        alpha = 255 - static_cast<int>(f * 255.0);
      }
      else
      {
        alpha = static_cast<int>(f * 255.0);
      }
      fill_color = sf::Color(255, 255, 255, alpha);
    }

    const screen_rect sprite_rect(square_layout.get_piece());

    draw_texture(
      get_piece_texture(
        piece.get_race(),
        piece.get_color(),
        piece.get_type()
      ),
      sprite_rect,
      fill_color
    );

    if (is_present_in(piece.get_id(), selected_piece_ids))
    {
      draw_outline(sprite_rect, to_sfml_color(piece.get_color()), 5);
    }

    // Show the piece is proteced
    if (indicate_protectedness
      && is_square_protected(game.get_pieces(), piece.get_current_square(), piece.get_color())
    ) {
      draw_texture(
        game_resources::get().get_board_game_textures().get_shield(),
        square_layout.get_is_protected(),
        sf::Color::Blue
      );
    }
  }
}

void draw_squares(
  const board_layout& layout,
  const bool semi_transparent
)
{
  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      const chess_color c{(x + y) % 2 == 0 ? chess_color::black : chess_color::white };
      const screen_rect square_rect{layout.get_square(x, y).get_square()};
      auto& t{game_resources::get().get_textures()};
      if (semi_transparent)
      {
        draw_texture(t.get_semitransparent_square(c), square_rect);
      }
      else
      {
        draw_texture(t.get_square(c), square_rect);
      }
    }
  }
}

void draw_unit_health_bars(
  const game& g,
  const board_layout& bl
)
{
  for (const auto& piece: g.get_pieces())
  {
    const auto& square_layout{
      bl.get_square(
        piece.get_current_square().get_x(),
        piece.get_current_square().get_y()
      )
    };
    const auto outline_rect{
      square_layout.get_health_bar_outline()
    };

    // Black background
    draw_rectangle(
      outline_rect,
      sf::Color::Black
    );

    try
    {
      const double f_health{get_f_health(piece)};

      // Will be lower for rooxx
      const auto bar_rect{square_layout.get_health_bar_value(f_health, piece.get_race())};
      const sf::Color health_color{f_health_to_color(get_f_health(piece))};
      draw_rectangle(
        bar_rect,
        health_color
      );
    }
    catch (std::logic_error&)
    {
      // OK
    }
    if (piece.get_race() == race::rooxx)
    {
      try
      {
        const double f_shield{get_f_shield(piece)};

        // Will be lower for rooxx
        const auto bar_rect{square_layout.get_shield_bar_value(f_shield)};
        const sf::Color shield_color{f_shield_to_color(get_f_shield(piece))};
        draw_rectangle(
          bar_rect,
          shield_color
        );
      }
      catch (std::logic_error&)
      {
        // OK
      }

    }
  }
}

void draw_unit_paths(
  const std::vector<piece>& pieces,
  const board_layout& layout
)
{
  for (const auto& piece: pieces)
  {
    if (is_idle(piece)) continue;

    const auto square_piece{
      layout.get_square(
        piece.get_current_square().get_x(),
        piece.get_current_square().get_y()
      ).get_square()
    };
    const auto piece_pixel{get_center(square_piece)};

    const auto& actions{piece.get_actions()};
    for (const auto& action: actions)
    {

      const auto square_from{
        layout.get_square(action.get_from().get_x(), action.get_from().get_y()).get_square()
      };
      const auto square_to{
        layout.get_square(action.get_to().get_x(), action.get_to().get_y()).get_square()
      };
      const auto from_pixel{get_center(square_from)};
      const auto to_pixel{get_center(square_to)};
      const auto center_pixel{(to_pixel + from_pixel) / 2.0};
      const double length{calc_distance(from_pixel, to_pixel)};
      const double angle_degrees{calc_angle_degrees(center_pixel, to_pixel)};
      sf::RectangleShape rect;
      assert(get_width(square_from) == get_width(square_to));
      const double max_square_height{static_cast<double>(get_width(square_piece))};
      const double height{std::max(2.0, max_square_height * 0.05)};
      rect.setSize(sf::Vector2f(length, height));
      rect.setOrigin(length / 2, height / 2);
      rect.setPosition(
        center_pixel.get_x(),
        center_pixel.get_y()
      );
      rect.rotate(-angle_degrees);
      rect.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
      rect.setOutlineThickness(2);
      rect.setFillColor(to_sfml_color(piece.get_color()));
      get_render_window().draw(rect);
    }



    // Collect all the coordinats for the path
    std::vector<screen_coordinate> coordinats;
    coordinats.reserve(piece.get_actions().size() + 1); // +1 for current position


    coordinats.push_back(piece_pixel);
    /*
    coordinats.push_back(
      convert_to_screen_coordinate(
        to_coordinat(piece.get_current_square()),
        layout
      )
    );
    */
    std::transform(
      std::begin(actions),
      std::end(actions),
      std::back_inserter(coordinats),
      [layout](const auto& user_input)
      {
        const auto square_to{
          layout.get_square(user_input.get_to().get_x(), user_input.get_to().get_y()).get_square()
        };
        const auto to_pixel{get_center(square_to)};
        return to_pixel;
        /*
        return convert_to_screen_coordinate(
          to_coordinat(user_input.get_to()),
          layout
        );
        */
      }
    );

    // Draw circles at the subgoals
    sf::CircleShape circle;
    for (const auto coordinat: coordinats)
    {
      const double full_diameter{static_cast<double>(get_width(square_piece))};
      const double diameter{0.25 * full_diameter};
      const double radius{diameter / 2.0};
      const double half_radius{radius/ 2.0};
      circle.setPosition(
        sf::Vector2f(
          coordinat.get_x() - half_radius,
          coordinat.get_y() - half_radius
        )
      );
      circle.setFillColor(to_sfml_color(piece.get_color()));
      circle.setRadius(radius);
      circle.setOrigin(half_radius, half_radius);
      get_render_window().draw(circle);
    }

    // Draw circle at current in-progress movement
    assert(!actions.empty());
    const auto& first_action{actions[0]};
    if (first_action.get_action_type() == piece_action_type::move
      || first_action.get_action_type() == piece_action_type::castle_kingside
      || first_action.get_action_type() == piece_action_type::castle_queenside
    )
    {
      const auto square_from{
        layout.get_square(first_action.get_from().get_x(), first_action.get_from().get_y()).get_square()
      };
      const auto square_to{
        layout.get_square(first_action.get_to().get_x(), first_action.get_to().get_y()).get_square()
      };
      const auto from_pixel{get_center(square_from)};
      const auto to_pixel{get_center(square_to)};
      /*
      const auto from_pixel{
        convert_to_screen_coordinate(
          to_coordinat(first_action.get_from()),
          layout
        )
      };
      const auto to_pixel{
        convert_to_screen_coordinate(
          to_coordinat(first_action.get_to()),
          layout
        )
      };
      */
      const auto f{piece.get_current_action_progress().get()};
      assert(f >= 0.0);
      assert(f <= 1.0);
      const auto delta_pixel{to_pixel - from_pixel};
      const auto now_pixel{from_pixel + (delta_pixel * f)};

      const double full_diameter{static_cast<double>(get_width(square_piece))};
      const double diameter{0.25 * full_diameter};
      const double radius{diameter / 2.0};
      const double half_radius{radius/ 2.0};
      circle.setPosition(
        sf::Vector2f(
          now_pixel.get_x() - half_radius,
          now_pixel.get_y() - half_radius
        )
      );
      circle.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
      circle.setOutlineThickness(std::max(2.0, radius / 10.0));
      circle.setFillColor(to_sfml_color(piece.get_color()));
      circle.setRadius(radius);
      circle.setOrigin(half_radius, half_radius);
      get_render_window().draw(circle);
    }
  }
}

#endif // LOGIC_ONLY
