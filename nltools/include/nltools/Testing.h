#pragma once
#include <csignal>

namespace nltools
{
  namespace test
  {
    static void _assert(bool test, const Glib::ustring& description)
    {
      if(!test)
      {
        std::cerr << description << std::endl;
      }
    }
  }
}