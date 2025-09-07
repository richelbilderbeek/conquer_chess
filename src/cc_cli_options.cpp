#include "cc_cli_options.h"

#include "helper.h"
#include "sfml_helper.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <iostream>

cc_cli_options::cc_cli_options(const std::vector<std::string>& args) :
  m_args{args}
{
  const int n_args = args.size();
  if (n_args) m_conquer_chess_exe_path = args[0];
  for (int i{1}; i<n_args; ++i)
  {
    if (!is_valid_cli_arg(args[i]))
    {
      std::stringstream s;
      s
        << "Invalid CLI argument: '" << args[i] << "'\n"
        << "Index: " << i << '\n'
        << "All arguments: " << to_comma_seperated_str(args) << '\n'
        << '\n'
      ;
      show_help(s);
      throw std::logic_error(s.str());
    }
    assert(is_valid_cli_arg(args[i]));
  }


  const bool do_profile = std::count(std::begin(args), std::end(args), "--profile");
  const bool do_not_profile{
    std::count(std::begin(args), std::end(args), "--no-profile")
    || std::count(std::begin(args), std::end(args), "--no_profile")
  };
  assert(!(do_profile && do_not_profile));
  if (do_profile) m_do_profile = true;
  if (do_not_profile) m_do_profile = false;

  const bool do_test = std::count(std::begin(args), std::end(args), "--test");
  const bool do_not_test{
    std::count(std::begin(args), std::end(args), "--no-test")
    || std::count(std::begin(args), std::end(args), "--no_test")
  };
  assert(!(do_test && do_not_test));
  if (do_test) m_do_test = true;
  if (do_not_test) m_do_test = false;

  const bool do_assert_to_log = std::count(std::begin(args), std::end(args), "--assert_to_log");
  if (do_assert_to_log) m_do_assert_to_log = true;

  const bool do_play_standard_random_game = std::count(std::begin(args), std::end(args), "--play_standard_random_game");
  if (do_play_standard_random_game) m_do_play_standard_random_game = true;

  const bool do_show_debug_info = std::count(std::begin(args), std::end(args), "--show_debug_info");
  if (do_show_debug_info) m_do_show_debug_info = true;

  const bool do_show_help = std::count(std::begin(args), std::end(args), "--help");
  if (do_show_help) m_do_show_help = true;

  const bool is_vbf_mode = std::count(std::begin(args), std::end(args), "--vbf_mode");
  if (is_vbf_mode) m_is_vbf_mode = true;
}

std::vector<std::string> collect_args(int argc, char **argv)
{
  std::vector<std::string> v(argv, argv + argc);
  return v;
}

std::vector<std::string> get_all_cli_args() noexcept
{
  const auto t{get_all_cli_args_and_descriptions()};
  std::vector<std::string> args;
  args.reserve(t.size());
  std::transform(
    std::begin(t), std::end(t), std::back_inserter(args),
    [](const auto& p) { return p.first; }
  );
  assert(args.size() == t.size());
  return args;
}

std::vector<std::pair<std::string, std::string>> get_all_cli_args_and_descriptions() noexcept
{
  return
  {
    { "--assert_to_log", "Redirect assert output to a log file" },
    { "--help", "Show help and quit" },
    { "--no-test", "[Deprecated]" },
    { "--no_test", "Do not run unit tests at startup" },
    { "--no-profile", "[Deprecated]" },
    { "--no_profile", "Do not acquire a run-time speed profile at startup" },
    { "--play_standard_random_game", "Play a standard random game at startup" },
    { "--profile", "Acquire a run-time speed profile at startup" },
    { "--show_debug_info", "Show the F3 debug info at startup" },
    { "--test", "Run the unit tests at startup" },
    { "--vbf_mode", "Run in a Virtual Bastard friendly mode" }
  };

}

bool is_valid_cli_arg(const std::string& arg)
{
  const auto valid_args{get_all_cli_args()};
  return std::find(std::begin(valid_args), std::end(valid_args), arg) != std::end(valid_args);
}

void show_help(std::ostream& os)
{
  os
    << "Conquer Chess help" << '\n'
    << "------------------" << '\n'
    << "Compile date: " << __DATE__ << '\n'
    << "Compile time: " << __TIME__ << '\n'
    << "SFML version: " << get_sfml_version() << '\n'
    << '\n'
  ;

  const auto t{get_all_cli_args_and_descriptions()};
  const auto longest_first_iter{
    std::max_element(
      std::begin(t),
      std::end(t),
      [](const auto& lhs, const auto& rhs) {
        return lhs.first.size() < rhs.first.size();
      }
    )
  };
  const auto max_length{
    (*longest_first_iter).first.size()
  };

  for (const auto& p: t)
  {
    const auto n_spaces{max_length - p.first.size()};

    os << p.first << " " << std::string(n_spaces, ' ') << p.second << '\n';
  }
}

void test_cli_options()
{
#ifndef NDEBUG
  // constructor
  {
    const cc_cli_options options;
    assert(options.get_do_test());
  }
  // constructor with invalid arguments
  {
    bool has_thrown{false};
    try
    {
      const cc_cli_options options( { "conquer_chess", "--nonsense" } );
    }
    catch (std::logic_error& e)
    {
      has_thrown = true;
    }
    assert(has_thrown);
  }
  // --assert_to_log
  {
    const cc_cli_options options_1;
    assert(!options_1.get_do_assert_to_log());
    const cc_cli_options options_2( { "--assert_to_log" } );
    assert(options_2.get_do_assert_to_log());
  }
  // --help
  {
    const cc_cli_options options_1;
    assert(!options_1.get_do_show_help());
    const cc_cli_options options_2( { "--help" } );
    assert(options_2.get_do_show_help());
  }
  // --play_standard_random_game
  {
    const cc_cli_options options_1;
    assert(!options_1.get_do_play_standard_random_game());
    const cc_cli_options options_2( { "--play_standard_random_game" } );
    assert(options_2.get_do_play_standard_random_game());
  }
  // --profile
  {
    const cc_cli_options options_1( { "--profile" } );
    assert(options_1.get_do_profile());
    const cc_cli_options options_2( { "--no-profile" } );
    assert(!options_2.get_do_profile());
    const cc_cli_options options_3( { "--no_profile" } );
    assert(!options_3.get_do_profile());
  }
  // --test
  {
    const cc_cli_options options_1( { "--test" } );
    assert(options_1.get_do_test());
    const cc_cli_options options_2( { "--no-test" } );
    assert(!options_2.get_do_test());
    const cc_cli_options options_3( { "--no_test" } );
    assert(!options_3.get_do_test());
  }
  // --vbf_mode
  {
    const cc_cli_options options_1;
    assert(!options_1.get_is_vbf_mode());
    const cc_cli_options options_2( { "--vbf_mode" } );
    assert(options_2.get_is_vbf_mode());
  }
  // get_conquer_chess_exe_path
  {
    const cc_cli_options options_1;
    assert(options_1.get_conquer_chess_exe_path().empty());
    const cc_cli_options options_2( { "my_folder/conquer_chess" } );
    assert(!options_2.get_conquer_chess_exe_path().empty());

  }
  // get_all_cli_args
  {
    const auto v{get_all_cli_args()};
    for (const auto& s: v)
    {
      assert(!s.empty());
    }
  }
  // get_all_cli_args_and_descriptions
  {
    const auto v{get_all_cli_args_and_descriptions()};
    for (const auto& p: v)
    {
      assert(!p.first.empty());
      assert(!p.second.empty());
    }
  }
  // get_is_compiled_in_debug_move
  {
    const cc_cli_options options;
    #ifndef NDEBUG
    assert(options.get_compiled_in_debug_mode());
    #else
    assert(!options.get_compiled_in_debug_mode());
    assert(1 == 2); // This will never happen :-)
    #endif
  }
  // is_valid_cli_arg
  {
    assert(is_valid_cli_arg("--profile"));
    assert(is_valid_cli_arg("--no-profile"));
    assert(is_valid_cli_arg("--play_standard_random_game"));
    assert(is_valid_cli_arg("--test"));
    assert(is_valid_cli_arg("--no-test"));
    assert(is_valid_cli_arg("--assert_to_log"));
    assert(is_valid_cli_arg("--show_debug_info"));
  }
  // show_help
  {
    std::stringstream s;
    show_help(s);
    assert(!s.str().empty());
  }
  // operator<<
  {
    std::stringstream s;
    s << cc_cli_options();
    assert(!s.str().empty());
  }
#endif // NDEBUG
}


std::ostream& operator<<(std::ostream& os, const cc_cli_options& options) noexcept
{
  os
    << "Conquer Chess executable path: " << options.m_conquer_chess_exe_path << '\n'
    << "Raw CLI arguments: " << to_comma_seperated_str(options.m_args) << '\n'
    << "Compiled in debug mode: " << bool_to_str(options.m_compiled_in_debug_move) << '\n'
    << "Redirect assert output to log: " << bool_to_str(options.m_do_assert_to_log) << '\n'
    << "Play a standard random game: " << bool_to_str(options.m_do_play_standard_random_game) << '\n'
    << "Show debug info at startup: " << bool_to_str(options.m_do_show_debug_info) << '\n'
    << "Run a run-time speed profile: " << bool_to_str(options.m_do_profile) << '\n'
    << "Run all unit tests: " << bool_to_str(options.m_do_test)
    << "VBF mode: " << bool_to_str(options.m_is_vbf_mode)
  ;
  return os;
}
