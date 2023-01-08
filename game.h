#ifndef GAME_H
#define GAME_H

#include "game_options.h"
#include "pieces.h"
#include "message.h"
#include "lobby_options.h"

#include <iosfwd>
#include <optional>
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
///
/// This class is part of an MVC Design Pattern:
///  * Model: \link{game}
///  * View: \link{game_view}
///  * Controller: \link{game_controller}
class game
{
public:
  explicit game(
    const game_options& go = create_default_game_options(),
    const lobby_options& lo = create_default_lobby_options()
  );

  /// Get the game options
  const auto& get_game_options() const noexcept { return m_game_options; }

  /// Get the game options
  const auto& get_lobby_options() const noexcept { return m_lobby_options; }

  /// Get all the pieces
  auto& get_pieces() noexcept { return m_pieces; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Get the in-game time
  const auto& get_time() const noexcept { return m_t; }

  /// Go to the next frame
  void tick(const delta_t& dt = delta_t(1.0));

private:

  /// The game options
  game_options m_game_options;

  /// The game options
  lobby_options m_lobby_options;

  /// All pieces in the game
  std::vector<piece> m_pieces;

  /// The time
  delta_t m_t;
};

/// Can this piece castle kingside?
bool can_castle_kingside(const piece& p, const game& g) noexcept;

/// Can this piece castle queenside?
bool can_castle_queenside(const piece& p, const game& g) noexcept;

/// Can an action be done?
bool can_do(const game& g,
  const piece& selected_piece,
  const piece_action_type action,
  const square& cursor_square,
  const side player_side
);

/// Can an action be done?
bool can_do(const game& g,
  const piece& selected_piece,
  const piece_action_type action,
  const std::string& cursor_square_str,
  const side player_side
);

/// Can a piece_action_type::attack action be done?
/// This is not an en-passant
/// @see use \link{can_do_en_passant} for an piece_action_type::en_passant
bool can_do_attack(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
);

/// Can a piece_action_type::castle_kingside action be done?
bool can_do_castle_kingside(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
);

/// Can a piece_action_type::castle_queenside action be done?
bool can_do_castle_queenside(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
);

/// Can a piece_action_type::en_passant action be done?
/// This is not a regular attack
/// @see use \link{can_do_attack} for an piece_action_type::attack
bool can_do_en_passant(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
);

/// Can a piece_action_type::move action be done?
bool can_do_move(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
);

/// Can a piece_action_type::promote_to_bishop action be done?
bool can_do_promote(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
);

/// Clear the sound effects to be processed,
/// i.e. resize to zero
void clear_piece_messages(game& g) noexcept;

/// Collect the history of a game,
/// i.e. the moves played in time
action_history collect_action_history(const game& g);

/// Collect all valid moves and attackes at a board
/// for all pieces
/// @see use 'collect_all_user_inputs'
/// to get all the 'user_input's from a game
std::vector<piece_action> collect_all_piece_actions(const game& g);

/// Collect all valid moves and attackes at a board
/// for all pieces of a certain color
std::vector<piece_action> collect_all_piece_actions(
  const game& g,
  const chess_color player_color
);

/// Collect all valid moves and attackes at a board
/// for a focal piece
std::vector<piece_action> collect_all_piece_actions(
  const game& g,
  const piece& p
);

/// Collect all valid moves and attacks at a board
/// for a focal bishop
std::vector<piece_action> collect_all_bishop_actions(
  const game& g,
  const piece& p
);

/// Collect all valid moves and attacks at a board
/// for a focal king
std::vector<piece_action> collect_all_king_actions(
  const game& g,
  const piece& p
);

/// Collect all valid moves and attacks at a board
/// for a focal knight
std::vector<piece_action> collect_all_knight_actions(
  const game& g,
  const piece& p
);

/// Collect all valid moves and attacks at a board
/// for a focal pawn
std::vector<piece_action> collect_all_pawn_actions(
  const game& g,
  const piece& p
);

/// Collect all valid attack actions at a board
/// for a focal pawn
std::vector<piece_action> collect_all_pawn_attack_actions(
  const game& g,
  const piece& p
);

/// Collect all valid attack actions at a board
/// for a focal pawn
std::vector<piece_action> collect_all_pawn_en_passant_actions(
  const game& g,
  const piece& p
);

/// Collect all valid move actions at a board
/// for a focal pawn
std::vector<piece_action> collect_all_pawn_move_actions(
  const game& g,
  const piece& p
);

/// Collect all valid moves and attacks at a board
/// for a focal queen
std::vector<piece_action> collect_all_queen_actions(
  const game& g,
  const piece& p
);

/// Collect all valid moves and attacks at a board
/// for a focal rook
std::vector<piece_action> collect_all_rook_actions(
  const game& g,
  const piece& p
);

/// Get all the sound effects to be processed
std::vector<message> collect_messages(const game& g) noexcept;

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

/// Count the number of selected units of a player side
int count_selected_units(
  const game& g,
  const side player_side
);

/// Create a randomly played game
game create_randomly_played_game(
  const int n_moves = 10,
  const int seed = 42
);

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

game get_default_game() noexcept;


/// Get the default, primary, most likely piece action
/// Returns an empty optional if the current setup
/// cannot result in an action
std::optional<piece_action_type> get_default_piece_action(
  const game& g,
  const square& cursor_square,
  const side player_side
) noexcept;

/// Get all the pieces in the starting position type
std::vector<piece> get_starting_pieces(
  const game_options& go,
  const lobby_options& lo
) noexcept;

/// Create a game with all default settings
/// and a specific starting position
game get_game_with_starting_position(starting_position_type t) noexcept;

/// Get the ID of a piece at a square
/// Will throw if there is no piece there
id get_id(const game& g, const square& s);

/// Get the index of the piece that is closest to the coordinat
int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
);

/// Create a game in which it is only a king versus a king,
/// to be used in debugging
game get_kings_only_game() noexcept;

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
const piece& get_piece_at(const game& g, const std::string& square_str);

/// Get the piece that at that square,
/// will throw if there is no piece
piece& get_piece_at(game& g, const square& coordinat);

/// Get the piece that at that square,
/// will throw if there is no piece
piece& get_piece_at(game& g, const std::string& square_str);

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

/// Get the time in the game
const delta_t& get_time(const game& g) noexcept;

/// See if there is at least 1 piece selected
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'get_selected_pieces' to get all the selected pieces
bool has_selected_pieces(const game& g, const chess_color player);

/// See if there is at least 1 piece selected
bool has_selected_pieces(const game& g, const side player);

/// Is the square empty?
bool is_empty(const game& g, const square& s) noexcept;

/// Are all squares between these two squares empty?
/// Returns true if the squares are adjacent
bool is_empty_between(
  const game& g,
  const square& from,
  const square& to
);

/// Are all squares between these two squares empty?
/// Returns true if the squares are adjacent
bool is_empty_between(
  const game& g,
  const std::string& from_square_str,
  const std::string& to_square_str
);

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

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game& g,
  const std::string& square_str
);

/// See if there is a piece with a certain ID at a certain square
bool piece_with_id_is_at(
  game& g,
  const id& i,
  const square& s
);

/// Call game::tick until all pieces are idle
void tick_until_idle(game& g);

/// Convert the played game to pseudo-PGN notation
/// Returns one string with newlines
std::string to_pgn(const game& g);

/// Unselect all pieces of a certain color
void unselect_all_pieces(
  game& g,
  const chess_color color
);

std::ostream& operator<<(std::ostream& os, const game& g) noexcept;

#endif // GAME_H
