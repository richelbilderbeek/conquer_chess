#ifndef PROGRAM_STATE_H
#define PROGRAM_STATE_H

#include <string>
#include <vector>

enum class program_state {
  about,
  game,
  left_controls,
  loading,
  lobby,
  main_menu,
  option_controls,
  option_laws,
  option_video_and_audio,
  replay,
  right_controls
};

/// Get all the program_state values
std::vector<program_state> get_all_program_states() noexcept;

/// Test this class and its free functions
void test_program_state();

std::string to_str(const program_state t) noexcept;

std::ostream& operator<<(std::ostream& os, const program_state t) noexcept;

#endif // PROGRAM_STATE_H
