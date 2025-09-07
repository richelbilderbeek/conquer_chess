#include "about.h"

#include <cassert>
#include <SFML/System/String.hpp>
#include <vector>
std::wstring get_author_name() noexcept
{
  return L"Richèl Bilderbeek";
}

std::string get_homepage_url() noexcept
{
  return "https://richelbilderbeek.github.io/conquer_chess/";
}

std::vector<sf::String> get_contributor_names() noexcept
{
  return {
    sf::String(L"Björn Engström"),
    sf::String(L"Herman Adsten"),
    sf::String(L"Johanna Söderström"),
    sf::String(L"Leonid Rutkowski")
  };
}

void test_about()
{
#ifndef NDEBUG
  // get_author_name
  {
    assert(get_author_name().length() > 10);
  }
  // get_homepage_url
  {
    assert(get_homepage_url().length() > 10);
  }
  // get_contributor_names
  {
    assert(get_contributor_names().size() == 4);
  }
#endif // NDEBUG
}

