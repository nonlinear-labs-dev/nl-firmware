#pragma once

#include <sstream>
#include <functional>

namespace nltools
{
  namespace string
  {
    template <typename... Args> std::string concat(Args... args)
    {
      std::stringstream s;
      (s << ... << args);
      return s.str();
    }
  }
  void parseURI(
      const std::string &uri,
      std::function<void(const std::string &scheme, const std::string &host, const std::string &path, uint port)> cb);

  bool startsWith(const std::string &string, const std::string &test);
}
