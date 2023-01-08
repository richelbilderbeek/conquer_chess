#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#ifndef LOGIC_ONLY

#include <string>

class game_resources;

/// Loads resources
class resource_loader
{
public:
  resource_loader();

  int get_n_items() const noexcept { return 11; }

  std::string get_current() const noexcept { return m_descriptor; }

  int get_index() const noexcept { return m_index; }

  bool is_done() const noexcept;

  void process_next(game_resources& resources);

private:

  std::string m_descriptor;

  int m_index;
};

double get_progress(const resource_loader& loader) noexcept;

#endif // LOGIC_ONLY

#endif // RESOURCE_LOADER_H
