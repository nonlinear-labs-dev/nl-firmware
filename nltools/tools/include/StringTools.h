#pragma once

#include <sstream>

template <typename... Args> std::string concat(Args... args)
{
  std::stringstream s;
  std::initializer_list<bool>{ (s << args, false)... };
  return s.str();
}
