#include "diagnostics_file.h"

#include "cc_cli_options.h"
#include "sfml_helper.h"

#include <chrono>
#include <fstream>
#include <ios>

#include <SFML/Config.hpp>

diagnostics_file::diagnostics_file(
  const std::string filename
) : m_filename{filename}
{

}

void diagnostics_file::add_cli_options(const cc_cli_options& options)
{
  std::ofstream f(m_filename, std::ios::app); // Append
  f
    << "---------------------------------------------------------------" << '\n'
    << options << '\n'
  ;
}

void diagnostics_file::add_footer()
{
  const auto now = std::chrono::system_clock::now();
  const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

  std::ofstream f(m_filename, std::ios::app); // Append
  f
    << "Game ended successfully at " << std::ctime(&now_time) << '\n'
  ;
}

void diagnostics_file::add_header()
{
  const auto now = std::chrono::system_clock::now();
  const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

  std::ofstream f(m_filename, std::ios::app); // Append
  f
    << " " << '\n'
    << "===============================================================" << '\n'
    << "Conquer Chess log file." << '\n'
    << "Compile date: " << __DATE__ << '\n'
    << "Compile time: " << __TIME__ << '\n'
    << "Current time and date: " << std::ctime(&now_time) << '\n'
    << "SFML version: " << get_sfml_version() << '\n'
  ;
}

void diagnostics_file::add_screen_size(const int width, const int height)
{
  std::ofstream f(m_filename, std::ios::app);
  f
    << "Screen size (width x height): " << width << " x " << height << '\n';
}

std::string get_default_diagnostics_filename()  noexcept
{
  return "conquer_chess_error.txt";
}
