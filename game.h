#ifndef GAME_H
#define GAME_H

#include "control_actions.h"
#include "game_coordinat.h"
#include "game_options.h"
#include "game_view_layout.h"
#include "piece.h"
#include "sound_effect.h"
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  game(
    const game_options& options = get_default_game_options()
  );

  /// Add an action. These will be processed in 'tick'
  void add_action(const control_action a);

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

  /// Get the in-game keyboard position
  const auto& get_player_1_pos() const noexcept { return m_player_1_pos; }

  /// Get the in-game mouse position
  const auto& get_player_2_pos() const noexcept { return m_player_2_pos; }

  /// Get the game options
  auto& get_options() noexcept { return m_options; }

  /// Get the game options
  const auto& get_options() const noexcept { return m_options; }

  /// Get all the pieces
  auto& get_pieces() noexcept { return m_pieces; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

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

  std::vector<piece> m_pieces;

  friend void test_game();
};

/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
);

/// Can the player select a piece at the current mouse position?
bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const chess_color player
);

/// Clear the sound effects to be processed,
/// i.e. resize to zero
void clear_sound_effects(game& g) noexcept;

/// Count the total number of actions to be done by the game,
/// which should be zero after each tick
int count_control_actions(const game& g);

/// Count the total number of actions to be done by pieces of a player
int count_piece_actions(
  const game& g,
  const chess_color player
);

/// Count the number of selected units of a player
int count_selected_units(
  const game& g,
  const chess_color player
);

/// Create a game in which it is only a king versus a king,
/// to be used in debugging
game create_king_versus_king_game();

/// Are selected squares shown on-screen?
bool do_show_selected(const game& g) noexcept;

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

/// Get the index of the piece that is closest to the coordinat
int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
);

/// Get the position of the player that uses the keyboard
game_coordinat get_keyboard_player_pos(const game& g);

/// Get the position of the player that uses the keyboard
game_coordinat& get_keyboard_player_pos(game& g);

/// Get the position of the player that uses the mouse
game_coordinat get_mouse_player_pos(const game& g);

/// Get the position of the player that uses the mouse
game_coordinat& get_mouse_player_pos(game& g);

/// Get the game options
game_options get_options(const game& g);

/// Get the piece that at that square,
/// will throw if there is no piece
const piece& get_piece_at(const game& g, const square& coordinat);

/// Get the piece that at that square,
/// will throw if there is no piece
piece& get_piece_at(game& g, const square& coordinat);

/// Get all the selected pieces
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
);

/// Get all the sound effects to be processed
const std::vector<sound_effect>& get_sound_effects(const game& g) noexcept;

/// Get all the pieces
const std::vector<piece>& get_pieces(const game& g) noexcept;

/// See if there is at least 1 piece selected
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'get_selected_pieces' to get all the selected pieces
bool has_selected_pieces(const game& g, const chess_color player);

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

/// Test this class and its free functions
void test_game();

/// Toggle the color of the active player
void toggle_left_player_color(game& g);

/// Unselect all pieces of a certain color
void unselect_all_pieces(
  game & g,
  const chess_color color
);

#endif // GAME_H
