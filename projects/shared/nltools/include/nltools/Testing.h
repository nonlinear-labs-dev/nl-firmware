#pragma once

#include <csignal>

namespace nltools
{
  namespace test
  {
    static bool printPassedTests = false;

    static void _assert(bool test, const Glib::ustring& description)
    {
      if(!test)
      {
        nltools::Log::error("[FAIL]", description);
        std::raise(SIGINT);
      }
      else if(printPassedTests)
      {
        nltools::Log::warning("[PASS]", description);
      }
    }

    template <typename T> static void assertEqualsSilent(const T& expected, const T& test, const char* description)
    {
      auto res = test == expected;
      if(!res)
      {
        nltools::Log::error("[FAIL]", description, " Not equal!");
        std::raise(SIGINT);
      }
      else if(printPassedTests)
      {
        nltools::Log::warning("[PASS]", description);
      }
    }

    template <typename T> static void assertEquals(const T& expected, const T& test, const char* description)
    {
      auto res = test == expected;
      if(!res)
      {
        nltools::Log::error("[FAIL] ", description, "expected:", expected, "got:", test);
        std::raise(SIGINT);
      }
      else if(printPassedTests)
      {
        nltools::Log::warning("[PASS]", description);
      }
    }

    template <typename T> static void assertNotNull(const T* ptr)
    {
      if(ptr == nullptr)
      {
        nltools::Log::error("[FAIL] got: nullptr expected: non null pointer");
        std::raise(SIGINT);
      }
      else if(printPassedTests)
      {
        nltools::Log::warning("[PASS] Pointer not Null");
      }
    }
  }
}
