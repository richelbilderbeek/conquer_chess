#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#ifndef LOGIC_ONLY

#include <string>

class game_resources;

/// Loads resources gradually, so that progress
/// can be displayed.
class resource_loader
{
public:
  resource_loader();

  /// The number of resource groups that need to be loaded.
  ///
  /// This value is tested to be correct.
  /// The failed assert will give a helpful error message.
  constexpr int get_n_items() const noexcept { return 23; }

  /// Describe the current progress
  std::string get_current() const noexcept { return m_descriptor; }

  /// Get the index of the current progress
  int get_index() const noexcept { return m_index; }

  /// Are all resources loaded?
  bool is_done() const noexcept;

  void process_next();

private:

  std::string m_descriptor;

  int m_index;

  bool m_is_done{false};
};

/// Get the progress of the loader
/// @return a value from
double get_progress(const resource_loader& loader) noexcept;

/// Test this class
void test_resource_loader();

#endif // LOGIC_ONLY

#endif // RESOURCE_LOADER_H
