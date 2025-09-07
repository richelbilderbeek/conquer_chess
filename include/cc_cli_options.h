#ifndef CC_CLI_OPTIONS_H
#define CC_CLI_OPTIONS_H

/// @file Conquer Chess Command Line Options
///
/// This file and class are prepended with 'cc_'
/// to get rid of errors.
/// It appeared that the 'cli_options'
///
///

#include <iosfwd>
#include <string>
#include <vector>

/// Command line options
class cc_cli_options
{
public:
  cc_cli_options(const std::vector<std::string>& args = {});

  /// Is Conquer Chess compiled in debug mode?
  auto get_compiled_in_debug_mode() const noexcept { return m_compiled_in_debug_move; }

  /// Get the path to the Conquer Chess executable
  auto get_conquer_chess_exe_path() const noexcept { return m_conquer_chess_exe_path; }

  /// On Steam, the output of assert must be put in an error log file,
  /// so that users can submit a bug report
  auto get_do_assert_to_log() const noexcept{ return m_do_assert_to_log; }

  auto get_do_profile() const noexcept { return m_do_profile; }

  /// Does the user want to see the help?
  auto get_do_show_help() const noexcept { return m_do_show_help; }

  auto get_do_test() const noexcept { return m_do_test; }
  auto get_do_play_standard_random_game() const noexcept { return m_do_play_standard_random_game; }

  /// Show the in-game debug info at the start
  auto get_do_show_debug_info() const noexcept { return m_do_show_debug_info; }

  /// Is the game in Virtual Bastard friendly mode?
  auto get_is_vbf_mode() const noexcept { return m_is_vbf_mode; }




private:

  /// The original CLI arguments
  std::vector<std::string> m_args;

  std::string m_conquer_chess_exe_path;

  #ifndef NDEBUG
  bool m_compiled_in_debug_move{true};
  #else
  bool m_compiled_in_debug_move{false};
  #endif


  bool m_do_assert_to_log{false};
  bool m_do_play_standard_random_game{false};
  bool m_do_profile{false};
  bool m_do_show_debug_info{false};
  bool m_do_show_help{false};
  bool m_do_test{true};
  bool m_is_vbf_mode{false};

  friend std::ostream& operator<<(std::ostream& os, const cc_cli_options& options) noexcept;

};

/// Get all the valid CLI arguments
std::vector<std::string> get_all_cli_args() noexcept;

/// Get all the valid CLI arguments and
std::vector<std::pair<std::string, std::string>> get_all_cli_args_and_descriptions() noexcept;

/// Is this a valid CLI argument?
bool is_valid_cli_arg(const std::string& arg);

/// Convert CLI arguments to string
std::vector<std::string> collect_args(int argc, char **argv);

/// Show the help
void show_help(std::ostream& os);


/// Test these functions
void test_cli_options();

std::ostream& operator<<(std::ostream& os, const cc_cli_options& options) noexcept;


#endif // CC_CLI_OPTIONS_H
