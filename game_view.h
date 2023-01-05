#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#ifndef LOGIC_ONLY

#include "ccfwd.h"
#include "physical_controller.h"
#include "game.h"
#include "fps_clock.h"
#include "game_log.h"
#include "game_resources.h"
#include "game_view_layout.h"

#include <SFML/Graphics.hpp>

#include <optional>

/// The game's main window
/// Displays the game class
class game_view
{
public:
  explicit game_view(
    const game& game = get_default_game(),
    const game_controller& c = game_controller()
  );

  /// Run the game, until the user quits
  void exec();

  /// The the elapsed time in seconds
  double get_elapsed_time_secs() const noexcept;

  int get_fps() const noexcept { return m_fps_clock.get_fps(); }

  auto& get_game() noexcept { return m_game; }

  const auto& get_game() const noexcept { return m_game; }

  const auto& get_game_controller() const noexcept { return m_game_controller; }

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_game_resources; }

  /// Get the text log, i.e. things pieces have to say
  const auto& get_log() const noexcept { return m_log; }

  auto& get_window() noexcept { return m_window; }

private:

  /// The game clock, to measure the elapsed time
  sf::Clock m_clock;

  /// The FPS clock
  fps_clock m_fps_clock;

  /// The game logic
  game m_game;

  /// The game controller, interacts with game
  game_controller m_game_controller;

  /// The game logic
  game_view_layout m_layout;

  /// The resources (images, sounds, etc.) of the game
  game_resources m_game_resources;

  /// The text log
  game_log m_log;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Play the new sound effects
  void play_pieces_sound_effects();

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Read the pieces' messages and play their sounds
  void process_piece_messages();

  /// Show the game on-screen
  void show();

  /// Show the mouse cursor on-screen
  void show_mouse_cursor();
};

/// Convert 'true' to 'true' and 'false' to 'false'
std::string bool_to_str(const bool b) noexcept;

/// Create a black/dark square at the right size
sf::RectangleShape create_black_square(game_view& view);

/// Create a white/ligt square at the right size
sf::RectangleShape create_white_square(game_view& view);

/// Are selected squares shown on-screen?
bool do_show_selected(const game_view& view);

/// Get the controller for a certain side
const physical_controller& get_physical_controller(const game_view& view, const side player);

/// Get the controller type for a certain side
physical_controller_type get_physical_controller_type(const game_view& view, const side player);

/// Get the controls text for a player with a controller
std::string get_controls_text(
  const game_view& view,
  const game_controller& c,
  const int key
);

/// Get the frames per second
int get_fps(const game_view& v) noexcept;

/// Get the last log messages for a player
std::string get_last_log_messages(
  const game_view& v,
  const side player
) noexcept;

/// Get the layout
const game_view_layout& get_layout(const game_view& v) noexcept;

/// Get the game options
const game_options& get_options(const game_view& v) noexcept;

/// Get the pieces
const std::vector<piece>& get_pieces(const game_view& v) noexcept;

/// Get the color of the player
chess_color get_player_color(
  const game_view& v,
  const side player
) noexcept;

/// Get the player position
const game_coordinat& get_cursor_pos(
  const game_view& view,
  const side player
) noexcept;

/// Get the text for an action, e.g.
/// 'Q\nSelect' for a keyboard player that has nothing selected
/// 'E\nAttack' for a keyboard player that has a piece select
/// 'LMB\nSelect' for a mouse player that has nothing selected'
std::string get_text_for_action(
  const game_view& view,
  const game_controller& c,
  const int key
);

/// Get the time in the game
const delta_t& get_time(const game_view& v) noexcept;

/// Process the event, by letting the controllers
/// add actions to the game
/// sf::Event -> controllers -> control_action
void process_event(
  game_controller& c,
  const sf::Event& event,
  const game_view_layout& layout
);

/// Show the board: squares, unit paths, pieces, health bars
void show_board(game_view& view);

/// Show the controls (e.g. for a unit) on-screen for a player
void show_controls(game_view& view, const side player);

/// Show debug info on-screen for a player
void show_debug(game_view& view, const side player_side);

/// Show the layout of the window: board and panels
void show_layout(game_view& view);

/// Show the log on-screen, i.e. things the pieces say
void show_log(game_view& view, const side player);

/// Show the squares that are occupied on-screen
/// Throws if this option is turned off
void show_occupied_squares(game_view& view);

/// Show the pieces of the board on-screen
void show_pieces(game_view& view);

/// Show the possible moves for the selected pieces.
/// e.g. put crosses on squares that can be attacked.
/// Does nothing if no pieces are selected
void show_possible_moves(game_view& view);

/// Show the info on the side-bar on-screen for a player
void show_sidebar(game_view& view, const side player_side);

/// Show the squares of the board on-screen
void show_squares(game_view& view);

/// Show the highlighted square under the cursor on-screen for a player
void show_square_under_cursor(
  game_view& view,
  const side player
);

/// Show the pieces' health bars on-screen
void show_unit_health_bars(game_view& view);

/// Show the planned paths for the units on-screen
void show_unit_paths(game_view& view);

/// Show the selected unit(s) on-screen for a player
void show_unit_sprites(game_view& view, const side player_side);

void test_game_view();

/*
/// Toggle the color of the active player
void toggle_left_player_color(game_view& view);
*/

/// Would it be a valid move if the user would press LMB or RMB?
bool would_be_valid(
  const game_view& view,
  chess_color player_color
);

#endif // LOGIC_ONLY

#endif // GAME_VIEW_H
