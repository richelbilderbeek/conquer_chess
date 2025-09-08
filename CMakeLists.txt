cmake_minimum_required(VERSION 3.19)
project(conquer_chess VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Static build settings (only for non-MSVC)
if(NOT MSVC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()


# Set vcpkg toolchain if not set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(SFGRAPHING_SRC
    ../SFGraphing/src/PieChartDataSet.cpp
    ../SFGraphing/src/PlotDataSet.cpp
    ../SFGraphing/src/SFPieChart.cpp
    ../SFGraphing/src/SFPlot.cpp
)

# Get ALL SFGraphing source files recursively
file(GLOB SFGRAPHING_SRC
    "../SFGraphing/src/*.cpp"
    "../SFGraphing/src/*.cxx"
    "../SFGraphing/src/*.cc"
    "../SFGraphing/src/*.c"
    "../SFGraphing/*.cpp"
    "../SFGraphing/*.cxx"
    "../SFGraphing/*.cc"
)

# Debug: Print found SFGraphing files
message(STATUS "Found SFGraphing files: ${SFGRAPHING_SRC}")

# Get local source files
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")



add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${SFGRAPHING_SRC}
    ${LOCAL_SRC}
)

target_include_directories(conquer_chess PRIVATE
    ../SFGraphing/include
    ../SFGraphing
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/../SFGraphing/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../SFGraphing
)

target_compile_definitions(conquer_chess PRIVATE
    USE_SFGRAPHING
)
# Tell CMake to use SFML from vcpkg
find_package(SFML COMPONENTS system window graphics network audio main CONFIG REQUIRED)



# Link against SFML targets (vcpkg uses lowercase names)
target_link_libraries(conquer_chess PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
    sfml-main
)



# Resources list (unchanged)
set(game_resources_resource_files
    "resources/fonts/16114_FuturistFixed-width.ttf"
    "resources/fonts/CodeSquaredRegular-AYRg.ttf"
    "resources/fonts/arial.ttf"
    "resources/songs/aura.ogg"
    "resources/songs/bliss.ogg"
    "resources/songs/heroes.ogg"
    "resources/songs/silence.ogg"
    "resources/songs/soothing.ogg"
    "resources/songs/wonderful_time.ogg"
    "resources/sound_effects/attacking_high.ogg"
    "resources/sound_effects/attacking_low.ogg"
    "resources/sound_effects/attacking_mid.ogg"
    "resources/sound_effects/bark_1.ogg"
    "resources/sound_effects/bark_2.ogg"
    "resources/sound_effects/countdown.ogg"
    "resources/sound_effects/done_high.ogg"
    "resources/sound_effects/done_low.ogg"
    "resources/sound_effects/done_mid.ogg"
    "resources/sound_effects/faring_into_battle.ogg"
    "resources/sound_effects/heu_high.ogg"
    "resources/sound_effects/heu_low.ogg"
    "resources/sound_effects/heu_mid.ogg"
    "resources/sound_effects/hide.ogg"
    "resources/sound_effects/hmm_high.ogg"
    "resources/sound_effects/hmm_low.ogg"
    "resources/sound_effects/hmm_mid.ogg"
    "resources/sound_effects/i_cannot_high.ogg"
    "resources/sound_effects/i_cannot_low.ogg"
    "resources/sound_effects/i_cannot_mid.ogg"
    "resources/sound_effects/i_cant_high.ogg"
    "resources/sound_effects/i_cant_low.ogg"
    "resources/sound_effects/i_cant_mid.ogg"
    "resources/sound_effects/its_time_to_rock.ogg"
    "resources/sound_effects/jumping_into_battle.ogg"
    "resources/sound_effects/lets_rule.ogg"
    "resources/sound_effects/moving_forward.ogg"
    "resources/sound_effects/no_high.ogg"
    "resources/sound_effects/no_low.ogg"
    "resources/sound_effects/no_mid.ogg"
    "resources/sound_effects/nope_high.ogg"
    "resources/sound_effects/nope_low.ogg"
    "resources/sound_effects/nope_mid.ogg"
    "resources/sound_effects/to_rule_is_to_act.ogg"
    "resources/sound_effects/yes_high.ogg"
    "resources/sound_effects/yes_low.ogg"
    "resources/sound_effects/yes_mid.ogg"
    "resources/textures/artwork/all_races_1.jpg"
    "resources/textures/artwork/all_races_2.jpg"
    "resources/textures/artwork/all_races_3.jpg"
    "resources/textures/artwork/all_races_4.jpg"
    "resources/textures/black_black.png"
    "resources/textures/black_black_semitransparent.png"
    "resources/textures/black_square.png"
    "resources/textures/black_square_semitransparent.png"
    "resources/textures/black_strip.png"
    "resources/textures/black_white.png"
    "resources/textures/black_white_semitransparent.png"
    "resources/textures/board_game/dice_shield.png"
    "resources/textures/board_game/dice_skull.png"
    "resources/textures/board_game/dice_sword.png"
    "resources/textures/board_game/dollar.png"
    "resources/textures/board_game/flag_square.png"
    "resources/textures/board_game/flag_triangle.png"
    "resources/textures/board_game/hourglass.png"
    "resources/textures/board_game/pawn_up.png"
    "resources/textures/board_game/shield.png"
    "resources/textures/board_game/skull.png"
    "resources/textures/board_game/sword.png"
    "resources/textures/input_prompts/input_prompt_textures.xml"
    "resources/textures/input_prompts/keyboard.png"
    "resources/textures/input_prompts/keyboard_0.png"
    "resources/textures/input_prompts/keyboard_0_outline.png"
    "resources/textures/input_prompts/keyboard_1.png"
    "resources/textures/input_prompts/keyboard_1_outline.png"
    "resources/textures/input_prompts/keyboard_2.png"
    "resources/textures/input_prompts/keyboard_2_outline.png"
    "resources/textures/input_prompts/keyboard_3.png"
    "resources/textures/input_prompts/keyboard_3_outline.png"
    "resources/textures/input_prompts/keyboard_4.png"
    "resources/textures/input_prompts/keyboard_4_outline.png"
    "resources/textures/input_prompts/keyboard_5.png"
    "resources/textures/input_prompts/keyboard_5_outline.png"
    "resources/textures/input_prompts/keyboard_6.png"
    "resources/textures/input_prompts/keyboard_6_outline.png"
    "resources/textures/input_prompts/keyboard_7.png"
    "resources/textures/input_prompts/keyboard_7_outline.png"
    "resources/textures/input_prompts/keyboard_8.png"
    "resources/textures/input_prompts/keyboard_8_outline.png"
    "resources/textures/input_prompts/keyboard_9.png"
    "resources/textures/input_prompts/keyboard_9_outline.png"
    "resources/textures/input_prompts/keyboard_a.png"
    "resources/textures/input_prompts/keyboard_a_outline.png"
    "resources/textures/input_prompts/keyboard_alt.png"
    "resources/textures/input_prompts/keyboard_alt_outline.png"
    "resources/textures/input_prompts/keyboard_any.png"
    "resources/textures/input_prompts/keyboard_any_outline.png"
    "resources/textures/input_prompts/keyboard_apostrophe.png"
    "resources/textures/input_prompts/keyboard_apostrophe_outline.png"
    "resources/textures/input_prompts/keyboard_arrow_down.png"
    "resources/textures/input_prompts/keyboard_arrow_down_outline.png"
    "resources/textures/input_prompts/keyboard_arrow_left.png"
    "resources极aries/input_prompts/keyboard_arrow_left_outline.png"
    "resources/textures/input_prompts/keyboard_arrow_right.png"
    "resources/textures/input_prompts/keyboard_arrow_right_out极.png"
    "resources/textures/input_prompts/keyboard_arrow_up.png"
    "resources/textures/input_prompts/keyboard_arrow_up_outline.png"
    "resources/textures/input极prompts/keyboard_arrows.png"
    "resources/textures/input_prompts/keyboard_arrows_all.png"
    "resources/textures/input_prompts/keyboard_arrows_down.png"
    "resources/textures/input_prompts/keyboard_arrows_down_outline.png"
    "resources/textures/input_prompts/keyboard_arrows_horizontal.png"
    "resources/textures/input_prompts/keyboard_arrows_horizontal_outline.png"
    "resources/textures/input_prompts/keyboard_arrows_left极.png"
    "resources/textures/input_prompts/keyboard_arrows_left_outline.png"
    "resources/textures/input_prompts/keyboard_arrows_none.png"
    "resources/textures/input_prompts/keyboard_arrows_right.png"
    "resources/textures/input_prompts/keyboard_arrows_right_outline.png"
    "resources/textures/input_prompts/keyboard_arrows_up.png"
    "resources/textures/input_prompts/keyboard_arrows_up_outline.png"
    "resources/textures/input_prompts/keyboard_arrows_vertical.png"
    "resources/textures/input_prompts/keyboard_arrows_vertical_outline.png"
    "resources/textures/input_prompts/keyboard_asterisk.png"
    "resources/textures/极input_prompts/keyboard_asterisk_outline.png"
    "resources/textures/input_prompts极keyboard_b.png"
    "resources/textures/input_prompts/keyboard_b_outline.png"
    "resources/textures/input_prompts/keyboard_backspace.png"
    "resources/textures/input_prompts/keyboard_backspace_icon.png"
    "resources/textures/input_prompts/keyboard_backspace_icon_alternative.png"
    "resources/textures/input_prompts/keyboard_backspace_icon_alternative_outline.png"
    "resources/textures/input_prompts/keyboard_backspace_icon_outline.png"
    "resources/textures/input_prompts/keyboard_backspace_outline.png"
    "resources/textures/input_prompts/keyboard_bracket_close.png"
    "resources/textures/input_prompts/keyboard_bracket_close_outline.png"
    "resources/textures/input_prompts/keyboard_bracket_greater.png"
    "resources/textures/input_prompts/keyboard_bracket_greater_outline.png"
    "resources/textures/input_prompts/keyboard_bracket_less.png"
    "resources/textures/input_prompts/keyboard_bracket_less_outline.png"
    "极resources/textures/input_prompts/keyboard_bracket_open.png"
    "resources/textures/input_prompts/keyboard_bracket_open_outline.png"
    "resources/textures/input_prompts/keyboard_c.png"
    "resources/textures/input_prompts/keyboard_c_outline.png"
    "resources/textures/input_prompts/keyboard_capslock.png"
    "resources/textures/input_prompts/keyboard_capslock_极icon.png"
    "resources/textures/input_prompts/keyboard_capslock_icon_outline.png"
    "resources/textures/input_prompts/keyboard_capslock_outline.png"
    "resources/textures/input_prompts/keyboard_caret.png"
    "resources/textures/input_prompts/keyboard_caret_outline.png"
    "resources/textures/input_prompts/keyboard_colon.png"
    "resources/textures/input_prompts/keyboard_colon_outline.png"
    "resources/textures/input_prompts/keyboard_comma.png"
    "resources/textures/input_prompts/keyboard_comma_outline.png"
    "resources/textures/input_prompts/keyboard_command.png"
    "resources/textures/input_prompts/keyboard_command_outline.png"
    "resources/textures/input_prompts/keyboard_ctrl.png"
    "resources/textures/input_prompts/keyboard_ctrl_outline.png"
    "resources/textures/input_prompts/keyboard_d.png"
    "resources/textures/input_prompts/keyboard_d_outline.png"
    "resources/textures/input_prompts/keyboard_delete.png"
    "resources/textures/input_prompts/keyboard_delete_outline.png"
    "resources/textures/input_prompts/keyboard_e.png"
    "resources/textures/input_prompts/keyboard极e_outline.png"
    "resources/textures/input_prompts/keyboard_end.png"
    "resources/textures/input_prompts/keyboard_end_outline.png"
    "resources/textures/input_prompts/keyboard_enter.png"
    "resources/textures/input_prompts/keyboard_enter_outline.png"
    "resources/textures/input_prompts/keyboard_equals.png"
    "resources/textures/input_prompts/keyboard_equals_outline.png"
    "resources/textures/input_prompts/keyboard_escape.png"
    "resources/textures/input_prompts/keyboard_escape_outline.png"
    "resources/textures/input_prompts/keyboard_exclamation.png"
    "resources/textures/input_prompts/keyboard_exclamation_outline.png"
    "resources/textures/input_prompts/keyboard_f.png"
    "resources/textures/input_prompts/keyboard_f1.png"
    "resources/textures/input_prompts/keyboard_f10.png"
    "resources/textures/input_prompts/keyboard极f10_outline.png"
    "resources/textures/input_prompts/keyboard_f11.png"
    "resources/textures/input_prompts/keyboard_f11_outline.png"
    "resources/textures/input_prompts/keyboard_f12极.png"
    "resources/textures/input_prompts/keyboard_f12_outline.png"
    "resources/textures/input_prompts/keyboard_f1_outline.png"
    "resources/textures/input_prompts/keyboard_f2.png"
    "resources/textures/input_prompts/keyboard_f2_outline.png"
    "resources/textures/input_prompts/keyboard_f3.png"
    "resources/textures/input_prompts/keyboard_f3_outline.png"
    "resources/textures/input_prompts/keyboard_f极4.png"
    "极resources/textures/input_prompts/keyboard_f4_outline.png"
    "resources/textures/input_prompts/keyboard_f5.png"
    "resources/textures/input_prompts/keyboard_f5_outline.png"
    "resources/textures/input_prompts/keyboard_f6.png"
    "resources/textures/input_prompts/keyboard_f6_outline.png"
    "resources/textures/input_prompts/keyboard_f7.png"
    "resources/textures/input_prompts/keyboard_f7_outline.png"
    "resources/textures/input_prompts/keyboard_f8.png"
    "resources/textures/input_prompts/keyboard_f8_outline.png"
    "resources/textures/input_prompts/keyboard_f9.png"
    "resources/textures/input_prompts/keyboard_f9_outline.png"
    "resources/textures/input_prompts/keyboard_f_outline.png"
    "resources/textures/input_prompts/keyboard_function.png"
    "resources/textures/input_prompts/keyboard_function_outline.png"
    "resources/textures/input_prompts/keyboard_g.png"
    "resources/textures/input_prompts/keyboard_g_outline.png"
    "resources/textures/input_prompts/keyboard_h.png"
    "resources/textures/input_prompts/keyboard_h_outline.png"
    "resources/textures/input_prompts/keyboard_home极.png"
    "resources/textures/input_prompts/keyboard_home_outline.png"
    "resources/textures极/input_prompts/keyboard_i.png"
    "resources/textures/input_prompts/keyboard_i_outline.png"
    "resources/textures/input_prompts/keyboard_insert.png"
    "resources/textures/input_prompts/keyboard_insert_outline.png"
    "resources/textures/input_prompts/keyboard_j.png"
    "resources/textures/input_prompts/keyboard_j_outline.png"
    "resources/textures/input_prompts/keyboard_k.png"
    "resources/textures/input_prompts/keyboard_k_outline.png"
    "resources/textures/input_prompts/keyboard_l.png"
    "resources/textures/input_prompts/keyboard_l_outline极.png"
    "resources/textures/input_prompts/keyboard_m.png"
    "resources/textures/input_prompts/keyboard_m_outline.png"
    "resources/textures/input_prompts/keyboard_minus.png"
    "resources/textures/input_prompts/keyboard_minus_outline.png"
    "resources/textures/input_prompts/keyboard_n.png"
    "resources/textures/input_prompts/keyboard_n_outline.png"
    "resources/textures/input_prompts/keyboard_numlock.png"
    "resources/textures/input_prompts/keyboard_numlock_outline.png"
    "resources/textures/input_prompts/keyboard_numpad_enter.png"
    "resources/textures/input_prompts/keyboard_numpad_enter_outline.png"
    "resources/textures/input_prompts/keyboard_numpad_plus.png"
    "resources/textures/input_prompts/keyboard_numpad_plus_outline.png"
    "resources/textures/input_prompts/keyboard_o.png"
    "resources/textures/input_prompts/keyboard_o_outline.png"
    "resources/textures/input_prompts/keyboard_option.png"
    "resources/textures/input_prompts/keyboard_option_outline.png"
    "resources/textures/input_prompts/keyboard_outline.png"
    "resources/textures/input_prompts/keyboard_p极.png"
    "resources/textures/input_prompts/keyboard_p_out极line.png"
    "resources/textures/input_prompts/keyboard_page_down.png"
    "resources/textures/input_prompts/keyboard_page_down_outline.png"
    "resources/textures/input_prompts/keyboard_page_up.png"
    "resources/textures/input_prompts/keyboard_page_up_outline.png"
    "resources/textures/input_prompts/keyboard_period.png"
    "resources/textures/input_prompts/keyboard_period_outline.png"
    "resources/textures/input_prompts/keyboard极plus.png"
    "resources/textures/input_prompts/keyboard_plus_outline.png"
    "resources/textures/input_prompts/keyboard_printscreen.png"
    "resources/textures/极input_prompts/keyboard_printscreen_outline.png"
    "resources/textures/input_prompts/keyboard_q.png"
    "resources/textures/input_prompts/keyboard_q_outline.png"
    "resources/textures/input_prompts/keyboard_question.png"
    "resources/textures/input_prompts/keyboard_question_outline.png"
    "resources/textures/input_prompts/keyboard_quote.png"
    "resources/textures/input_prompts/keyboard_quote_outline.png"
    "resources/textures/input_prompts/keyboard_r.png"
    "resources/textures/input_prompts/keyboard_r_outline.png"
    "resources/textures/input_prompts/keyboard_return.png"
    "resources/textures/input_prompts/keyboard_return_outline.png"
    "resources/textures/input_prompts/keyboard_s.png"
    "resources/textures/input_prompts/keyboard_s_outline.png"
    "resources/textures/input_prompts/keyboard_semicolon.png"
    "resources/textures/input_prompts/keyboard_semicolon_outline.png"
    "resources/textures/input_prompts/keyboard_shift.png"
    "resources/textures/input_prompts/keyboard_shift_icon.png"
    "resources/textures/input_prompts/keyboard_shift_icon极outline.png"
    "resources/textures/input_prompts/keyboard极shift_outline.png"
    "resources/textures/input_prompts/keyboard_slash_back极.png"
    "resources/textures/input_prompts/keyboard_slash_back_outline.png"
    "resources/textures/input_prompts/keyboard_slash_forward.png"
    "resources/textures/input_prompts/keyboard_slash_forward_outline.png"
    "resources/textures/input_prompts/keyboard_space.png"
    "resources/textures/input_prompts/keyboard_space_icon.png"
    "resources/textures/input_prompts/keyboard_space_icon_outline.png"
    "resources/textures/input_prompts/keyboard_space_outline.png"
    "resources/textures/极input_prompts/keyboard_t.png"
    "resources/textures/input_prompts/keyboard_t_outline.png"
    "resources/textures/input_prompts/keyboard_tab.png"
    "resources/textures/input_prompts/keyboard极tab_icon.png"
    "resources/textures/input_prompts/keyboard_tab_icon_alternative.png"
    "resources/textures/input_prompts/keyboard_tab极icon_alternative_outline.png"
    "resources/textures/input_prompts/keyboard_tab_icon_outline.png"
    "resources/textures/input_prompts/keyboard_tab_outline.png"
    "resources/textures/input_prompts/keyboard_tilde.png"
    "resources/textures/input_prompts/keyboard_tilde_outline.png"
    "resources/textures/input_prompts/keyboard_u.png"
    "resources/textures/input_prompts/keyboard_u_outline.png"
    "resources/textures/input_prompts/keyboard_v.png"
    "resources/textures/input_prompts/keyboard_v_outline.png"
    "resources/textures/input_prompts/keyboard_w.png"
    "resources/textures/input_prompts/keyboard_w_outline.png"
    "resources/textures/input_prompts/keyboard_win.png"
    "极resources/textures/input_prompts/keyboard_win_outline.png"
    "resources/textures/input_prompts/keyboard_x.png"
    "resources/textures/input_prompts/keyboard_x_outline.png"
    "resources/textures/input_prompts/keyboard_y.png"
    "resources/textures/input_prompts/keyboard_y_outline.png"
    "resources/textures/input_prompts/keyboard_z.png"
    "resources/textures/input_prompts/keyboard_z_outline.png"
    "resources/textures/input_prompts/mouse.png"
    "resources/textures/input_prompts/mouse_horizontal.png"
    "resources/textures/input_prompts/mouse_left.png"
    "resources/textures/input_prompts/mouse_left_outline.png"
    "resources/textures/input_prompts/mouse_move.png"
    "resources/textures/input_prompts/mouse_outline.png"
    "resources/textures/input_prompts/m极ouse_right.png"
    "resources/textures/input_prompts/mouse_right_outline.png"
    "resources/textures/input_prompts/mouse_scroll.png"
    "resources/textures/input_prompts/mouse_scroll_down.png"
    "resources/textures/input_prompts/mouse_scroll_down_outline.png"
    "resources/textures/input_prompts/mouse_scroll_outline.png"
    "resources/textures/input_prompts/mouse_scroll_up.png"
    "resources/textures/input_prompts/mouse_scroll_up_outline.png"
    "resources/textures/input_prompts/mouse_scroll_vertical.png"
    "resources/textures/input_prompts/mouse_scroll_vertical_outline.png"
    "resources/textures/input_prompts/mouse_small.png"
    "resources/textures/input_prompts/mouse_vertical.png"
    "resources/textures/lobby_menu/black_chess_color.jpg"
    "resources/textures/lobby_menu/class极ic_head.jpg"
    "resources/textures/lobby_menu/kingdom_head.jpg"
    "resources/textures/lobby_menu/ready_no.jpg"
    "resources/textures/lobby_menu/ready_yes.jpg"
    "resources/textures/lobby_menu/rooxx_head.jpg"
    "resources/textures/lobby_menu/spawn_head.jpg"
    "resources/textures/lobby_menu/white_chess_color.jpg"
    "resources/textures/maps/classic_map.jpg"
    "resources/textures极/maps/kingdom_map.jpg"
    "resources/textures/maps/rooxx_map.jpg"
    "resources/textures/maps/spawn_map.jpg"
    "resources/textures/options_menu/game_speed.jpg"
    "resources/textures/options_menu/left_controls.jpg"
    "resources/textures/options_menu/music_volume.jpg"
    "resources/textures/options_menu/right_controls.jpg"
    "resources/textures/options_menu/sound_effects_volume.jpg"
    "resources/textures/options_menu/starting_position.jpg"
    "resources/textures/physical_controller/fancy_keyboard.png"
    "resources/textures/physical_controller/fancy_mouse.png"
    "resources/textures/physical_controller/keyboard.png"
    "resources/textures/physical_controller/mouse.png"
    "resources/textures/piece_actions/attack.png"
    "resources/textures/piece_actions/attack_en_passant.png"
    "resources/textures/piece_actions/castle_kingside.png"
    "resources/textures/piece_actions/castle_queenside.png"
    "极resources/textures/piece_actions/move.png"
    "resources/textures/piece_actions/promote_to_bishop.png"
    "resources/textures/piece极actions/promote_to_knight.png"
    "resources/textures/piece_actions/promote_to_queen.png"
    "resources/textures/piece_actions/promote_to_rook.png"
    "resources/textures/piece_actions/select.png"
    "resources/textures/piece_actions/unselect.png"
    "resources/textures/pieces/classic_black_bishop.png"
    "resources/textures/pieces/classic_black_bishop_with_contour.png"
    "resources/textures/pieces/classic_black_king.png"
    "resources/textures/pieces/classic_black_king_with_contour.png"
    "resources/textures/pieces/classic_black_knight极.png"
    "resources/textures/pieces/classic_black_knight_with_contour.png"
    "resources/textures/pieces/classic极black_pawn.png"
    "resources/textures/pieces/classic_black_pawn_with_contour.png"
    "resources/textures/pieces/classic_black_queen.png"
    "resources/textures/pieces/classic_black_queen_with_contour.png"
    "resources/textures/pieces/classic_black_rook.png"
    "resources/textures/pieces/classic_black_rook_with_contour.png"
    "resources/textures/pieces/classic_white_bishop.png"
    "resources/textures/pieces/classic_white_bishop_with_contour.png"
    "resources/textures/pieces/classic_white_king.png"
    "resources/textures/pieces/classic_white_king_with_contour.png"
    "resources/textures/pieces/classic_white_knight.png"
    "resources/textures/pieces/classic_white_knight_with_contour.png"
    "resources/textures/pieces/classic_white_pawn.png"
    "resources/textures/pieces/classic_white_pawn_with_contour.png"
    "resources/textures/pieces/classic_white_queen.png"
    "resources/textures/p极ieces/classic_white_queen_with_contour.png"
    "resources/textures/pieces/classic_white_rook.png"
    "resources/textures/pieces/classic_white_rook_with_contour.png"
    "resources/textures/pieces/kingdom_black_bishop.jpg"
    "resources/textures/pieces/kingdom_black_king.jpg"
    "resources/textures/pieces/kingdom_black_knight.jpg"
    "resources/textures/pieces/kingdom_black_pawn.jpg"
    "resources/textures/pieces/kingdom_black_queen.jpg"
    "resources/textures/pieces/kingdom_black_rook.jpg"
    "resources/textures/pieces/kingdom_white_bishop.jpg"
    "resources/textures/pieces/kingdom_white_king.jpg"
    "resources/textures/pieces/kingdom_white_knight.jpg"
    "resources/textures/pieces/kingdom_white_pawn.jpg"
    "resources/textures/pieces/kingdom_white_queen.jpg"
    "resources/textures/pieces/kingdom_white_rook.jpg"
    "resources/textures/pieces/rooxx_black_bishop.jpg"
    "resources/textures/pieces/rooxx极_black_king.jpg"
    "resources/textures/pieces/rooxx_black_knight.jpg"
    "resources/textures/pieces/rooxx_black_pawn.jpg"
    "resources/textures/pieces/rooxx_black_queen极.jpg"
    "resources/textures/pieces/rooxx_black_rook.jpg"
    "resources/textures/pieces/rooxx_white_bishop.jpg"
    "resources/textures/pieces/rooxx_white_king.jpg"
    "resources/textures/pieces/rooxx_white_knight.jpg"
    "resources/textures/pieces/rooxx_white_pawn.jpg"
    "resources/textures/pieces/rooxx_white_queen.jpg"
    "resources/textures/pieces/rooxx_white_rook.jpg"
    "resources/textures/pieces/spawn_black_bishop.jpg"
    "resources/textures/pieces/spawn_black_king.jpg"
    "resources/textures/pieces/spawn_black_knight.jpg"
    "resources/textures/pieces/spawn_black_pawn.jpg"
    "resources/textures/pieces/spawn_black_queen.jpg"
    "resources/textures/pieces/spawn_black_rook.jpg"
    "resources/textures/pieces/spawn_white_bishop.jpg"
    "resources/textures/pieces/spawn_white_king.jpg"
    "resources/textures/pieces/spawn_white_knight.jpg"
    "resources/textures/pieces/spawn_white_pawn.jpg"
    "resources/textures/pieces/spawn_white_queen.jpg"
    "resources/textures/pieces/spawn_white_rook极.jpg"
    "resources/textures/portraits/classic_black_bishop_portrait.jpg"
    "resources/textures/portraits/classic_black_king_portrait.jpg"
    "resources/textures/portraits/classic_black_knight_portrait.jpg"
    "resources/textures/portraits/classic_black_pawn_portrait.jpg"
    "resources/textures/portraits/classic_black_queen_portrait.jpg"
    "resources/textures/portraits/classic_black_rook_portrait.jpg"
    "resources/textures极/portraits/class极ic_white_bishop_portrait.jpg"
    "resources/textures/portraits/classic_white_king_portrait.jpg"
    "resources/textures/portraits/classic_white_knight_portrait极.jpg"
    "resources/textures/portraits/classic_white_pawn_portrait.jpg"
    "resources/textures/portraits/classic_white_queen_portrait.jpg"
    "resources/textures/portraits/classic_white_rook_portrait.jpg"
    "resources/textures/portraits/kingdom_black_bishop_portrait.jpg"
    "resources/textures/portraits/kingdom_black_king_portrait.jpg"
    "resources/textures/portraits/kingdom_black_knight_portrait.jpg"
    "resources/textures/portraits极/kingdom_black_pawn_portrait.jpg"
    "resources/textures/portraits/kingdom_black_queen_portrait.jpg"
    "resources/textures/portraits/kingdom_black_rook_portrait.jpg"
    "resources/textures/portraits/kingdom_white_bishop_portrait.jpg"
    "resources/textures/portraits/kingdom_white_king_portrait.jpg"
    "resources/textures/portraits/kingdom_white_knight_portrait.jpg"
    "resources/textures/portraits/kingdom_white_pawn_portrait.jpg"
    "resources/textures/portraits/kingdom_white_queen_portrait.jpg"
    "resources/textures/portraits/kingdom_white_rook_portrait.jpg"
    "resources/textures/portraits/rooxx_black_bishop_portrait.jpg"
    "resources/textures/portraits/rooxx_black_king_portrait.jpg"
    "resources/textures/portraits/rooxx_black_knight_portrait.jpg"
    "resources/textures/portraits/rooxx_black_pawn_portrait.jpg"
    "resources/textures/portraits/rooxx_black_queen_portrait.jpg"
    "resources/textures/portraits/rooxx_极black_rook_portrait.jpg"
    "resources/textures/portraits/rooxx_white_bishop_portrait.jpg"
    "极resources/textures/portraits/rooxx_white_king_portrait.jpg"
    "resources/textures/portraits/rooxx_white_knight_portrait.jpg"
    "resources/textures/portraits/rooxx_white_pawn_portrait.jpg"
    "resources/textures/portraits/rooxx_white_queen_portrait.jpg"
    "resources/textures/portraits/rooxx_white_rook_portrait.jpg"
    "resources/textures/portraits/spawn_black_bishop_portrait极.jpg"
    "resources/textures/portraits/spawn_black_king_portrait.jpg"
    "resources/textures/极portraits/spawn_black_knight_portrait.jpg"
    "resources/textures/portraits/spawn_black_pawn_portrait.jpg"
    "resources/textures/portraits/spawn_black_queen_portrait.jpg"
    "resources/textures/portraits/spawn_black_rook_portrait.jpg"
    "resources/textures/portraits/spawn_white_bishop_portrait.jpg"
    "resources/textures/portraits/spawn_white_king_portrait.jpg"
    "resources/textures/portraits/spawn_white_knight_portrait.jpg"
    "resources/textures/portraits/spawn_white_pawn_portrait.jpg"
    "resources/textures/portraits/spawn_white_queen_portrait.jpg"
    "resources/textures/portraits/spawn_white_rook_portrait.jpg"
    "resources/textures/qm.png"
    "resources/textures/quit.png"
    "resources/textures/subtitle.png"
    "resources/textures/t.png"
    "resources/textures/themba/themba_1.jpg"
    "resources/textures/themba/themba_2.jpg"
    "resources/textures/themba/themba_3极.jpg"
    "resources/textures/themba/themba_4.jpg"
    "resources/textures/themba/themba_5.jpg"
    "极resources/textures/title.png"
    "resources/textures/white_black.png"
    "resources/textures/white极_black_semitransparent.png"
    "resources/textures/white_square.png"
    "resources/textures/white_square_semitransparent.png"
    "resources/textures/white_strip.png"
    "resources/textures/white_white.png"
    "resources/textures/white_white_semitransparent.png"
)

if(MSVC)
    add_compile_options(/bigobj)
else()
    add_compile_options(-Wa,-mbig-obj)
endif()

# Add .qrc files
# Copy resources (instead of Qt .qrc system)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Fixed compile options with proper scope
if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(conquer_chess PRIVATE
        NDEBUG
    )
endif()

# Fixed compile options with proper scope
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(conquer_chess PRIVATE gcov)

    target_compile_options(conquer_chess PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -fprofile-arcs
        -ftest-coverage
    )
endif()

if(WIN32)
    target_include_directories(conquer_chess PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/include
    )
    
    target_include_directories(conquer_chess PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../chess-library/include
    )

    target_include_directories(conquer_chess PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../SFGraphing/include
    )
endif()

install(TARGETS conquer_chess
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

