#include "replay_view.h"

#ifndef LOGIC_ONLY

#include "game_statistics_widget.h"
#include "render_window.h"
#include "game_resources.h"
#include "draw_board.h"
#include "game.h"
#include <cassert>
#include <cmath>
#include <fstream>

// From https://github.com/jerr-it/SFGraphing
#include "../SFGraphing/include/SFGraphing/SFPlot.h"

replay_view::replay_view()
{
  m_controls_bar.set_draw_player_controls(false);
}

bool replay_view::process_event_impl(sf::Event& event)
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
    else if (key_pressed == sf::Keyboard::Key::F2)
    {
      std::ofstream file("replay.pgn");
      file << to_pgn(m_replay.get_game()) << '\n';
      play_sound_effect(message(message_type::done, chess_color::white, piece_type::king));
    }
  }
  return false; // Do not close the window :-)
}

void replay_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_coordinate br(event.size.width, event.size.height);

  const screen_rect window_rect(screen_coordinate(0,0), br);
  m_layout = replay_view_layout(window_rect);
  m_controls_bar.set_screen_rect(create_controls_bar_area(br));
}


void replay_view::draw_impl()
{
  draw_board(*this);
  draw_statistics(*this);
  draw_game_statistics_widget(
    this->m_layout.get_statistics_widget(),
    this->m_replay.get_game_controller()
  );
  m_controls_bar.draw();
}

void replay_view::set_replay(const replay& r)
{
  m_replay = r;
  m_statistics = extract_game_statistics_in_time(m_replay, delta_t(0.02));
  m_replay.reset();
}

void replay_view::start_impl()
{
  assert(!is_active());
  set_is_active(true);
  m_replay.reset();
}

void replay_view::stop_impl()
{
  assert(is_active());
  clear_next_state();
  set_is_active(false);
}

void replay_view::tick_impl(const delta_t dt)
{
  assert(dt.get() > 0.0);

  // Replay the game :-)
  m_replay.do_move(dt);
}

void draw_board(replay_view& v)
{
  const auto& g{v.get_game()};
  const board_layout& layout{v.get_layout().get_board()};
  const bool semi_transparent{false};
  draw_squares(layout, semi_transparent);
  draw_unit_paths(g.get_pieces(), layout);
  const bool indicate_protectedness{true};
  draw_pieces(
    v.get_replayer().get_game_controller(),
    layout,
    indicate_protectedness
  );
  draw_unit_health_bars(g, layout);
}

void draw_statistics(replay_view& v)
{
  // An empty replay
  if (v.get_statistics().get().empty()) return;

  const auto screen_rect{v.get_layout().get_statistics()};


  std::vector<float> times;
  std::vector<float> lhs_piece_values;
  std::vector<float> rhs_piece_values;
  std::vector<float> lhs_activity;
  std::vector<float> rhs_activity;
  std::vector<float> lhs_protectedness;
  std::vector<float> rhs_protectedness;
  for (const auto& s: v.get_statistics().get())
  {
    times.push_back(s.get(game_statistic_type::time, side::lhs));
    lhs_piece_values.push_back(s.get(game_statistic_type::value, side::lhs));
    rhs_piece_values.push_back(s.get(game_statistic_type::value, side::rhs));
    lhs_activity.push_back(s.get(game_statistic_type::activity, side::lhs));
    rhs_activity.push_back(s.get(game_statistic_type::activity, side::rhs));
    lhs_protectedness.push_back(s.get(game_statistic_type::protectedness, side::lhs));
    rhs_protectedness.push_back(s.get(game_statistic_type::protectedness, side::rhs));
  }

  csrc::PlotDataSet lhs_piece_values_in_time(times, lhs_piece_values, sf::Color(255, 128, 128), "", csrc::PlottingType::LINE);
  csrc::PlotDataSet rhs_piece_values_in_time(times, rhs_piece_values, sf::Color(255, 0, 0), "", csrc::PlottingType::LINE);
  csrc::PlotDataSet lhs_activity_in_time(times, lhs_activity, sf::Color(128, 255, 128), "", csrc::PlottingType::LINE);
  csrc::PlotDataSet rhs_activity_in_time(times, rhs_activity, sf::Color(0, 255, 0), "", csrc::PlottingType::LINE);
  csrc::PlotDataSet lhs_protectedness_in_time(times, lhs_protectedness, sf::Color(128, 128, 255), "", csrc::PlottingType::LINE);
  csrc::PlotDataSet rhs_protectedness_in_time(times, rhs_protectedness, sf::Color(0, 0, 255), "", csrc::PlottingType::LINE);

  const float t = v.get_replayer().get_game_controller().get_game().get_in_game_time().get();
  std::vector<float> vertical_bar_xs{ t, t };
  std::vector<float> vertical_bar_ys{ 0.0, 1.0};

  csrc::PlotDataSet vertical_bar_at_current_time(vertical_bar_xs, vertical_bar_ys, sf::Color(255, 255, 255), "", csrc::PlottingType::LINE);

  //Position, dimension, margin, font
  csrc::SFPlot plot(
    sf::Vector2f(screen_rect.get_tl().get_x(), screen_rect.get_tl().get_y()),
    sf::Vector2f(get_width(screen_rect), get_height(screen_rect)),
    32,
    get_arial_font(),
    "Time (chess moves)",
    "Fraction"
  );
  plot.AddDataSet(lhs_piece_values_in_time);
  plot.AddDataSet(rhs_piece_values_in_time);
  plot.AddDataSet(lhs_activity_in_time);
  plot.AddDataSet(rhs_activity_in_time);
  plot.AddDataSet(lhs_protectedness_in_time);
  plot.AddDataSet(rhs_protectedness_in_time);
  plot.AddDataSet(vertical_bar_at_current_time);

  //x-minimum, x-maximum, y-minimum, y-maximum, x-step-size, y-step-size, Color of axes
  assert(!times.empty());
  const double xmin{0.0};
  const double xmax{times.back()};
  const double ymin{0.0};
  const double ymax{1.0};
  const double tick_x{1.0};
  const double tick_y{0.1};
  plot.SetupAxes(xmin, xmax, ymin, ymax, tick_x, tick_y, sf::Color::White);
  plot.GenerateVertices();
  get_render_window().draw(plot);
}

#endif // LOGIC_ONLY
