#ifndef GAME_H
#define GAME_H


#include "ccfwd.h"
#include "message.h"
#include "piece.h"
#include "pieces.h"
#include "action_history.h"
#include "starting_position_type.h"
#include "fen_string.h"

#include <iosfwd>
#include <vector>
#include <optional>

/// The game logic.
///
/// All data types used by this class are STL and/or Boost
///
/// This class is part of an MVC Design Pattern:
///  * Model: \link{game}
///  * View: \link{game_view}
///  * Controller: \link{game_controller}
///
/// The \link{game_options} are a singleton
class game
{
public:
  explicit game(const std::vector<piece>& pieces = get_standard_starting_pieces());

  /// Get all the pieces
  auto& get_pieces() noexcept { return m_pieces; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Get the in-game time.
  /// The game and pieces should agree on the in-game time.
  const auto& get_in_game_time() const noexcept { return m_in_game_time; }

  const auto& get_winner() const noexcept { return m_winner; }

  /// Go to the next frame.
  ///
  /// The maximum timestep is 0.25 chess moves.
  /// Use the global `tick` function for timesteps
  /// longer than 0.25 chess moves.
  void tick(const delta_t& dt);

private:

  /// All pieces in the game
  std::vector<piece> m_pieces;

  /// The central game time
  in_game_time m_in_game_time;

  /// The winner, if any
  std::optional<chess_color> m_winner;

  /// At the start (and end) of a tick,
  /// all occupied squares must be unique.
  /// During a tick, as this is processed per piece,
  /// it may be that some squares are occupied twice,
  /// because a first piece already occupies it,
  /// where the second has not left it yet.
  void check_all_occupied_squares_are_unique() const;


  /// Both the game and the pieces keep track of the time.
  /// game::m_t is the cenral game time,
  /// and piece::m_time is the piece's idea of what time it is.
  /// These should be equal at the start and end of a tick
  void check_game_and_pieces_agree_on_the_time() const;

  /// Check if there is a winner
  void check_if_there_is_a_winner();

  /// Go to the next frame.
  ///
  /// The maximum timestep is 0.25 chess moves.
  void tick_impl(const delta_t& dt);

  friend game create_game_with_starting_position(
    starting_position_type t,
    const race lhs_race,
    const race rhs_race
  ) noexcept;
};

/// Can this piece castle kingside?
bool can_castle_kingside(const piece& p, const game& g) noexcept;

/// Can this piece castle queenside?
bool can_castle_queenside(const piece& p, const game& g) noexcept;

/// Can a piece_action_type::attack action be done?
/// This is not an en-passant
/// @see use \link{can_do_en_passant} for an piece_action_type::en_passant
bool can_do_attack(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const chess_color player_color
);

/// Can a piece_action_type::castle_kingside action be done?
bool can_do_castle_kingside(
  const game& g,
  const piece& selected_piece,
  const chess_color player_color
);

/// Can a piece_action_type::castle_queenside action be done?
bool can_do_castle_queenside(
  const game& g,
  const piece& selected_piece,
  const chess_color player_color
);

/// Can a piece_action_type::en_passant action be done?
/// This is not a regular attack
/// @see use \link{can_do_attack} for an piece_action_type::attack
bool can_do_en_passant(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const chess_color player_color
);

/// Can a piece_action_type::move action be done?
bool can_do_move(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const chess_color player_color
);

/// Can a piece_action_type::promote_to_bishop action be done?
bool can_do_promote(
  const piece& selected_piece,
  const chess_color player_color
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

/// See if there are messages
///
/// @seealso \link{collect_messages} to get the complete message
std::vector<message_type> collect_message_types(const game& g) noexcept;

/// Count the total number of actions to be done by pieces of both players
int count_piece_actions(const game& g);

/// Count the total number of actions to be done by pieces of a player
int count_piece_actions(
  const game& g,
  const chess_color player
);

/// Create a game from a FEN string
game create_game_from_fen_string(const fen_string& s) noexcept;

/// Create a game with all default settings
/// and the default starting position
game create_game_with_standard_starting_position() noexcept;

/// Create a game with all default settings
/// and a specific starting position
game create_game_with_starting_position(
  starting_position_type t,
  const race lhs_race = race::classic,
  const race rhs_race = race::classic
) noexcept;

/// Find zero, one or more chess pieces of the specified type and color
std::vector<piece> find_pieces(
  const game& g,
  const piece_type type,
  const chess_color color
);

/// Get the piece that is closest to the coordinat
const piece& get_closest_piece_to(const game& g, const game_coordinate& coordinat);

/// Get the piece that is closest to the coordinat
piece& get_closest_piece_to(game& g, const game_coordinate& coordinat);



/// Get the index of the piece that is closest to the coordinat
int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinate& coordinat
);

/*
/// Create a game in which it is only a king versus a king,
/// to be used in debugging
game get_kings_only_game() noexcept;
*/
/// Get all the squares that are occupied, disallowing duplicates
///
/// At the start and end of a tick, all occupied squares must be unique.
/// However, when updating each piece at a time, it can be that
/// some squares are duplicate untill all pieces are processed.
///
/// All squares are tested to be unique,
/// use \link{get_occupied_squares} to
/// allow duplicate squares.
std::vector<square> get_unique_occupied_squares(const game& g) noexcept;

/// Get all the squares that are occupied, allowing duplicates
///
/// At the start and end of a tick, all occupied squares must be unique.
/// However, when updating each piece at a time, it can be that
/// some squares are duplicate untill all pieces are processed.
///
/// All squares are not tested to be unique,
/// use \link{get_unique_occupied_squares} to
/// guarantee that all squares are unique.

std::vector<square> get_occupied_squares(const game& g) noexcept;

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

/// Get the piece with a certain ID.
/// Assumes that the piece is on the board
const piece& get_piece_with_id(const game& g, const piece_id& id);

/// Get the piece with a certain ID.
/// Assumes that the piece is on the board
piece& get_piece_with_id(game& g, const piece_id& id);

/// Get all the pieces
std::vector<piece>& get_pieces(game& g) noexcept;

/// Get all the pieces
const std::vector<piece>& get_pieces(const game& g) noexcept;

/// Get the time in the game
const in_game_time& get_time(const game& g) noexcept;

/// Determine if there is a piece at the coordinat
bool has_piece_with_id(
  const game& g,
  const piece_id& id
);

/// Determine if the game is a draw
bool is_draw(const game& g);

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
  const game_coordinate& coordinat,
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

/// Determine if the square is attacked by (another) piece of a certain color.
///
/// If the square is empty, it will determine if an enemy piece
/// is looking at this square.
///
/// If the square is occupied, it will determine if an enemy piece
/// is looking at that piece.
/// That piece can be of any color.
bool is_square_attacked(
  const game& g,
  const square& s,
  const chess_color attacker_color
);

/// Call game::tick safely.
///
/// That is, with a maximum delta t of 0.25
void tick(game& g, const delta_t dt);

/// Call game::tick until all pieces are idle
void tick_until_idle(game& g);

std::string to_board_str(
  const game& g,
  const board_to_text_options& options = board_to_text_options()
) noexcept;

/// Convert the game's position to a FEN string
fen_string to_fen_string(const game& g);

/// Convert the played game to pseudo-PGN notation.
///
/// Returns one string with newlines.
/// An example string can be: '0: white pawn move from e2 to e4',
/// where the '0' denotes the starting time of the move
std::string to_pgn(const game& g);

std::ostream& operator<<(std::ostream& os, const game& g) noexcept;

#endif // GAME_H
