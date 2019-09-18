#include <nltools/StringTools.h>
#include <libsoup/soup.h>
#include <algorithm>

namespace nltools
{
  void parseURI(
      const std::string &uri,
      std::function<void(const std::string &scheme, const std::string &host, const std::string &path, uint port)> cb)
  {
    auto u = soup_uri_new(uri.c_str());
    cb(soup_uri_get_scheme(u), soup_uri_get_host(u), soup_uri_get_path(u), soup_uri_get_port(u));
    soup_uri_free(u);
  }

  bool startsWith(const std::string &string, const std::string &test)
  {
    return g_str_has_prefix(string.c_str(), test.c_str());
  }
}
