#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#ifndef LOGIC_ONLY

#include "ccfwd.h"
#include "cc_cli_options.h"
#include "controls_bar.h"
#include "physical_controller.h"
//#include "game.h"
#include "game_log.h"
#include "game_controller.h"
#include "game_view_layout.h"
#include "game_statistics_output_file.h"
#include "game_statistics_in_time.h"
#include "view.h"

#include <SFML/Graphics.hpp>

//#include <optional>

/// The Game dialog.
///
/// The Game dialog shows the game and
/// displays the \link{game} class.
/// Here is where most of the action happens.
class game_view : public view
{
public:
  explicit game_view();

  /// Show the game on-screen
  void draw_impl() override;

  /// Run the game, until the user quits
  void tick_impl(delta_t dt) override;

  /// The the elapsed time in seconds
  double get_elapsed_time_secs() const noexcept;

  const auto& get_game() const noexcept { return m_game_controller.get_game(); }

  const auto& get_game_controller() const noexcept { return m_game_controller; }

  const auto& get_game_options() const noexcept { return m_game_options; }

  const auto& get_layout() const noexcept { return m_layout; }

  /// Get the text log, i.e. things pieces have to say
  const auto& get_log() const noexcept { return m_log; }

  const auto& get_physical_controllers() const noexcept { return m_pc; }

  [[nodiscard]] replay get_replay() const;

  /// Process all events
  /// @return if the user wants to quit
  bool process_event_impl(sf::Event& event) override;

  /// Process a resize events
  void process_resize_event_impl(sf::Event& event) override;

  /// Reset the game
  void reset();

  void set_cli_options(const cc_cli_options& cli_options) noexcept { m_cli_options = cli_options; }

  void set_game_options(const game_options& go) noexcept { m_game_options = go; }

  void set_lobby_options(const lobby_options& lo) noexcept;

  void start_impl() override;

  void stop_impl() override;

private:

  /// The game clock, to measure the elapsed time
  sf::Clock m_clock;

  cc_cli_options m_cli_options;

  controls_bar m_controls_bar;

  game_options m_game_options;

  /// The game controller, interacts with game
  game_controller m_game_controller;

  /// The game logic
  game_view_layout m_layout;

  lobby_options m_lobby_options;

  /// The in-game text log
  game_log m_log;

  physical_controllers m_pc;

  game_statistics_output_file m_statistics_output_file;

  game_statistics_in_time m_statistics_in_time;


  /// Play the new sound effects
  void play_pieces_sound_effects();

  /// Read the pieces' messages and play their sounds
  void process_piece_messages();

  /// Show the mouse cursor on-screen
  void show_mouse_cursor();
};

/// Create a black/dark square at the right size
sf::RectangleShape create_black_square(game_view& view);

/// Create a white/ligt square at the right size
sf::RectangleShape create_white_square(game_view& view);

/// Get the controller for a certain side
const physical_controller& get_physical_controller(const game_view& view, const side player);

/// Get the controller type for a certain side
physical_controller_type get_physical_controller_type(const game_view& view, const side player);

/// Get the controls text for a player with a controller
std::vector<std::string> get_controls_texts(
  const game_controller& c,
  const side player_side
);

/// Get the last log messages for a player
std::string get_last_log_messages(
  const game_view& v,
  const side player
) noexcept;

/// Get the layout
const game_view_layout& get_layout(const game_view& v) noexcept;

/// Get the pieces
const std::vector<piece>& get_pieces(const game_view& v) noexcept;

/// Get the player position
const game_coordinate& get_cursor_pos(
  const game_view& view,
  const side player
) noexcept;

/// Get the time in the game
const in_game_time& get_time(const game_view& v) noexcept;

/// Process the event, by letting the controllers
/// add actions to the game
/// sf::Event -> controllers -> control_action
void process_event(
  game_controller& c,
  const physical_controllers& pc,
  const sf::Event& event,
  const game_view_layout& layout
);


/// Show the map of the window
void draw_background(game_view& view);

/// Show the board: squares, unit paths, pieces, health bars
void draw_board(
  game_view& view,
  const bool show_occupied
);

/// Show the controls (e.g. for a unit) on-screen for a player
void draw_controls(
  game_view& view,
  const side player
);

/// Show the game info, e.g. the time
void draw_game_statistics_widget(game_view& view);

/// Show the controls (e.g. for a unit) on-screen for a player
void draw_navigation_controls(game_view& view);

/// Show debug info on-screen for a player
void show_debug(game_view& view, const side player_side);

/// Show the log on-screen, i.e. things the pieces say
void draw_log(game_view& view, const side player);


/// Show the squares that are occupied on-screen
/// Throws if this option is turned off
void show_occupied_squares(game_view& view);

/// Show the pieces of the board on-screen
void draw_pieces(game_view& view);

/// Show the possible moves for the selected pieces.
/// e.g. put crosses on squares that can be attacked.
/// Does nothing if no pieces are selected
void draw_possible_moves(game_view& view);

/// Show the info on the side-bar on-screen for a player
void show_sidebar(game_view& view, const side player_side);

/// Show the squares of the board on-screen
void draw_squares(game_view& view);

/// Show the highlighted square under the cursor on-screen for a player
void draw_cursor(
  game_view& view,
  const side player
);

/// Show the pieces' health bars on-screen
void draw_unit_health_bars(game_view& view);

/// Show the planned paths for the units on-screen
void draw_unit_paths(game_view& view);

/// Show the selected unit(s) on-screen for a player
void draw_unit_info(game_view& view, const side player_side);

void test_game_view();

/// Would it be a valid move if the user would press LMB or RMB?
bool would_be_valid(
  const game_view& view,
  chess_color player_color
);

#endif // LOGIC_ONLY

#endif // GAME_VIEW_H
