#ifndef GAME_H
#define GAME_H

#include "ccfwd.h"
#include "control_actions.h"
#include "game_coordinat.h"
#include "game_options.h"
#include "game_view_layout.h"
#include "pieces.h"
#include "message.h"
#include "replayer.h"
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  explicit game(
    const game_options& options = create_default_game_options()
  );

  /// Add an action. These will be processed in 'tick'
  void add_action(const control_action a);

  /// Do a chess move instantaneously
  void do_move(const chess_move& m);

  /// Get the game actions
  const auto& get_actions() const noexcept { return m_control_actions; }

  /// Get the game actions
  auto& get_actions() noexcept { return m_control_actions; }

  /// Get the position of the player that uses the keyboard
  game_coordinat& get_keyboard_player_pos();

  /// Get the layout of the screen
  const auto& get_layout() const noexcept { return m_layout; }

  /// Get the layout of the screen
  auto& get_layout() noexcept { return m_layout; }

  /// Get the position of the player that uses the mouse
  game_coordinat& get_mouse_player_pos();

  /// Get the player position
  const game_coordinat& get_player_pos(const side player) const noexcept;

  /// Get the game options
  auto& get_options() noexcept { return m_options; }

  /// Get the game options
  const auto& get_options() const noexcept { return m_options; }

  /// Get all the pieces
  auto& get_pieces() noexcept { return m_pieces; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Get the in-game time
  const auto& get_time() const noexcept { return m_t; }

  /// Go to the next frame
  void tick(const delta_t& dt = delta_t(1.0));

private:

  control_actions m_control_actions;

  /// The layout of the screen, e.g. the top-left of the sidebar
  game_view_layout m_layout;

  /// The in-game coordinat of the keyboard user
  game_coordinat m_player_1_pos;

  /// The in-game coordinat of the mouse
  game_coordinat m_player_2_pos;

  /// The game options
  game_options m_options;

  /// All pieces in the game
  std::vector<piece> m_pieces;

  /// Replay a match. Can be an empty match
  replayer m_replayer;

  /// The time
  delta_t m_t;

  friend void test_game();
};

/// Can the player select a piece at the current mouse position?
bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const chess_color player
);

/// Clear the sound effects to be processed,
/// i.e. resize to zero
void clear_piece_messages(game& g) noexcept;

/// Get all the sound effects to be processed
std::vector<message> collect_messages(const game& g) noexcept;

/// Count the total number of actions to be done by the game,
/// which should be zero after each tick
int count_control_actions(const game& g);

/// Count the total number of actions to be done by pieces of both players
int count_piece_actions(const game& g);

/// Count the total number of actions to be done by pieces of a player
int count_piece_actions(
  const game& g,
  const chess_color player
);

/// Count the number of selected units in total
int count_selected_units(const game& g);

/// Count the number of selected units of a player
int count_selected_units(
  const game& g,
  const chess_color player
);

/// Let the keyboard player move a piece
/// from the current selected square to a new target
/// @see 'do_select_and_move_keyboard_player_piece' does both
/// 'do_select_for_keyboard_player' and 'do_move_keyboard_player_piece'
void do_move_keyboard_player_piece(game& g, const square& s);

/// Let the keyboard player select a square
/// (assuming that a piece of the right color is there)
/// and let it move to another square
/// @see 'do_select_and_move_keyboard_player_piece' does both
/// 'do_select_for_keyboard_player' and 'do_move_keyboard_player_piece'
void do_select_and_move_keyboard_player_piece(
  game& g,
  const square& from,
  const square& to
);


/// Let the keyboard player select a square
/// (assuming that a piece of the right color is there)
/// and let it start an attack on another square
/// @see 'do_select_and_start_attack_keyboard_player_piece' does both
/// 'do_select_for_keyboard_player' and 'do_start_attack_keyboard_player_piece'
void do_select_and_start_attack_keyboard_player_piece(
  game& g,
  const square& from,
  const square& to
);

/// Let the keyboard player select the square
/// Assumes that a piece of the right color is there
/// @see 'do_select_and_move_keyboard_player_piece' does both
/// 'do_select_for_keyboard_player' and 'do_move_keyboard_player_piece'
void do_select_for_keyboard_player(game& g, const square& s);

/// Are selected squares shown on-screen?
bool do_show_selected(const game& g) noexcept;

/// Let the keyboard player attack a piece
/// from the current selected square to a new target
/// @see 'do_select_and_start_attack_keyboard_player_piece' does both
/// 'do_select_for_keyboard_player' and 'do_start_attack_keyboard_player_piece'
void do_start_attack_keyboard_player_piece(game& g, const square& s);

/// Find zero, one or more chess pieces of the specified type and color
std::vector<piece> find_pieces(
  const game& g,
  const piece_type type,
  const chess_color color
);

/// Get the piece that is closest to the coordinat
const piece& get_closest_piece_to(const game& g, const game_coordinat& coordinat);

/// Get the piece that is closest to the coordinat
piece& get_closest_piece_to(game& g, const game_coordinat& coordinat);

/// Get the cursor position for a player
game_coordinat get_cursor_pos(
  const game& g,
  const chess_color c
);

game get_default_game() noexcept;

/// Create a game with all default settings
/// and s specific starting position
game get_game_with_starting_position(starting_position_type t) noexcept;

/// Get the ID of a piece at a square
/// Will throw if there is no piece there
id get_id(const game& g, const square& s);

/// Get the index of the piece that is closest to the coordinat
int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
);

/// Get the position of the player that uses the keyboard
game_coordinat get_keyboard_player_pos(const game& g);

/// Get the position of the player that uses the keyboard
game_coordinat& get_keyboard_player_pos(game& g);

/// Get the color of the keyboard using player
/// Will throw if no user uses a keyboard
chess_color get_keyboard_user_player_color(const game& g);

/// Get the side of the controller that uses the keyboard.
/// Assumes there is one controller that uses the keyboard
side get_keyboard_user_player_side(const game& g);

/// Get the key for action 1, 2, 3 or 4 for a player
sf::Keyboard::Key get_key_for_action(const game& g, const side player, const int action);

/// Create a game in which it is only a king versus a king,
/// to be used in debugging
game get_kings_only_game() noexcept;

/// Get the layout
const game_view_layout& get_layout(const game& g) noexcept;

/// Get the position of the player that uses the mouse
game_coordinat get_mouse_player_pos(const game& g);

/// Get the position of the player that uses the mouse
game_coordinat& get_mouse_player_pos(game& g);

/// Get the color of the mouse user
chess_color get_mouse_user_player_color(const game& g);

/// Get the music volume as a percentage
double get_music_volume_as_percentage(const game& g) noexcept;

/// Get all the squares that are occupied
std::vector<square> get_occupied_squares(const game& g) noexcept;

/// Get the game options
const game_options& get_options(const game& g);

/// Get the piece that at that square,
/// will throw if there is no piece
const piece& get_piece_at(const game& g, const square& coordinat);

/// Get the piece that at that square,
/// will throw if there is no piece
piece& get_piece_at(game& g, const square& coordinat);

/// Get the piece that moves
piece& get_piece_that_moves(game& g, const chess_move& move);

/// Find a piece with a certain ID
/// Will throw if there is no piece with that ID
piece get_piece_with_id(
  const game& g,
  const id& i
);

/// Get the color of a player
chess_color get_player_color(
  const game& g,
  const side player
) noexcept;

/// Get the side of a player
side get_player_side(const game& g, const chess_color& color) noexcept;

/// Get the possible moves for a player's selected pieces
/// Will be empty if no pieces are selected
std::vector<square> get_possible_moves(
  const game& g,
  const side player
);

/// Get all the selected pieces
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
);

/// Get all the selected pieces
/// @param g a game
/// @param side the side of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const game& g,
  const side player
);

/// Get all the pieces
std::vector<piece>& get_pieces(game& g) noexcept;

/// Get all the pieces
const std::vector<piece>& get_pieces(const game& g) noexcept;

/// Get the player position
const game_coordinat& get_player_pos(const game& g, const side player) noexcept;

/// Get the time in the game
const delta_t& get_time(const game& g) noexcept;

/// See if there is at least 1 piece selected
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'get_selected_pieces' to get all the selected pieces
bool has_selected_pieces(const game& g, const chess_color player);

/// Are all pieces idle?
bool is_idle(const game& g) noexcept;

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game& g,
  const game_coordinat& coordinat,
  const double distance = 0.5
);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game& g,
  const square& coordinat
);

/// See if there is a piece with a certain ID at a certain square
bool piece_with_id_is_at(
  game& g,
  const id& i,
  const square& s
);

/// The the cursor of the keyboard player to the desired square
void set_keyboard_player_pos(
  game& g,
  const square& s
);

/// Call game::tick until all pieces are idle
void tick_until_idle(game& g);

/// Toggle the color of the active player
void toggle_left_player_color(game& g);

/// Unselect all pieces of a certain color
void unselect_all_pieces(
  game& g,
  const chess_color color
);

#endif // GAME_H
