#ifndef OPTION_LAWS_VIEW_H
#define OPTION_LAWS_VIEW_H

#ifndef LOGIC_ONLY

#include "ccfwd.h"
#include "chess_color.h"
#include "options_laws_view_layout.h"
#include "options_laws_view_item.h"
#include "game_options.h"
#include "physical_controllers.h"
#include "controls_bar.h"
#include "view.h"

#include <SFML/Graphics.hpp>

/// The Options dialog.
class options_laws_view : public view
{
public:
  explicit options_laws_view();

  /// Draw the menu on the main window
  void draw_impl() override;

  const auto& get_game_options() const noexcept { return m_game_options; }

  const auto& get_layout() const noexcept { return m_layout; }

  const auto& get_physical_controllers() const noexcept { return m_pc; }

  options_laws_view_item get_selected() const noexcept { return m_selected; }

  /// Has the user accepted these settings?
  ///
  /// This is signalled by how this window is closed:
  /// - Enter: yes, accept
  /// - Escape: no, reject, keep old game options
  bool has_accepted() const noexcept { return m_has_accepted; }

  /// Process all events
  /// @return if the user wants to quit
  bool process_event_impl(sf::Event& event) override;

  /// Process a resize event
  void process_resize_event_impl(sf::Event& event) override;

  /// Set the game options.
  ///
  /// This is used by the main window, that holds the game options
  /// that are used in the rest of the game.
  void set_game_options(const game_options& go) noexcept { m_game_options = go; }

  /// Select an item.
  /// Play a sound when the selected item changes
  void set_selected(const options_laws_view_item i);

  void start_impl() override;

  void stop_impl() override;

  void tick_impl(const delta_t dt) override;

private:

  /// Has the user accepted these settings?
  ///
  /// This is signalled by how this window is closed:
  /// - Enter: yes, accept
  /// - Escape: no, reject, keep old game options
  bool m_has_accepted{false};

  controls_bar m_controls_bar;

  game_options m_game_options;

  options_laws_view_layout m_layout;

  /// The physical controllers to display this window
  physical_controllers m_pc;

  /// The selected item
  options_laws_view_item m_selected;

  /// Increase the value in the selected square,
  /// can be done by leftarrow key
  void decrease_selected();

  /// Increase the value in the selected square,
  /// can be done by LMB, space and right arrow key
  void increase_selected();

};

void draw_panel(
  options_laws_view& v,
  const screen_rect& panel_position,
  const std::string panel_text,
  const chess_color color
);

/// Show the bottom part
void draw_background(options_laws_view& v);

/// Show the bottom part
void draw_bottom(options_laws_view& v);

/// Show the header of the bottom part,
/// i.e. 'Side' and 'Controller'
void draw_bottom_header(options_laws_view& v);

/// Show the left player's settings of the bottom part,
/// i.e. 'Left' and the LHS controller
void draw_bottom_row(options_laws_view& v, const side player_side);

/// Show the 'When to make a move' label and value
void draw_when_to_make_a_move(options_laws_view& v);

/// Show the music volume label and value
void draw_music_volume(options_laws_view& v);

/// Show all the layout panels
void draw_layout_panels(options_laws_view& v);

/// Show all the pieces in the initial position
void draw_pieces(options_laws_view& view);

/// Show the sound effects volume label and value
void draw_sound_effects_volume(options_laws_view& v);

/// Show the starting position label and value
void draw_starting_position(options_laws_view& v);

/// Show the selected panel
void draw_selected_panel(options_laws_view& v);

/// Show the top part
void draw_top(options_laws_view& v);

#endif // LOGIC_ONLY

#endif // OPTION_LAWS_VIEW_H
