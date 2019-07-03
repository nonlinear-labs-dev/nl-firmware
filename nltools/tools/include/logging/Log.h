#pragma once

#include <iostream>

namespace nltools
{
  template <typename... Args> void print(Args... args)
  {
    std::initializer_list<bool>{ (std::cout << args, false)... };
    std::cout << std::endl;
  }

  template <typename... Args> void warn(Args... args)
  {
    print(args...);
  }

  template <typename... Args> void error(Args... args)
  {
    print(args...);
  }

  template <typename... Args> void notify(Args... args)
  {
    print(args...);
  }
}
