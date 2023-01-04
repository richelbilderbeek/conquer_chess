#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <string>
#include "ccfwd.h"

/// Loads resources
class resource_loader
{
public:
  resource_loader();

  int get_n_items() const noexcept { return 1000; }

  std::string get_current() const noexcept { return "resources"; }

  int get_index() const noexcept { return m_index; }

  bool is_done() const noexcept;

  void process_next(game_resources& resources);

private:

  int m_index;
};

double get_progress(const resource_loader& loader) noexcept;

#endif // RESOURCE_LOADER_H
