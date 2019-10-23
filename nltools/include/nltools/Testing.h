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
        nltools::Log::error("[FAIL]", description);
        std::raise(SIGINT);
      }
    }

    template <typename T> static void assertEquals(const T& expected, const T& test)
    {
      auto res = test == expected;
      if(!res)
      {
        nltools::Log::error("[FAIL] expected:", expected, "got:", test);
        std::raise(SIGINT);
      }
    }

    template <typename T> static void assertNotNull(const T* ptr)
    {
      if(ptr == nullptr)
      {
        nltools::Log::error("[FAIL] got: nullptr expected: non null pointer");
      }
    }
  }
}