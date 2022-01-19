#include <nltools/StringTools.h>
#include <libsoup/soup.h>
#include <algorithm>
#include <fstream>
#include <nltools/logging/Log.h>
#include <vector>

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

  std::string getFileContent(const std::string &path)
  {
    std::ifstream stream(path);
    std::stringstream ss;
    std::string line;

    if(stream.is_open())
    {
      while(std::getline(stream, line))
      {
        ss << line;
      }
    }
    else
    {
      nltools::Log::error("could not open", path, "for reading");
    }

    return ss.str();
  }

  std::vector<unsigned char> readBinaryFile(const std::string &path)
  {
    std::ifstream input(path, std::ios::binary);
    if(input.is_open())
    {
      std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
      return buffer;
    }
    else
    {
      return {};
    }
  }

  namespace string
  {
    std::string removeCharacters(const std::string& in, std::vector<char> c)
    {
      std::string out;
      std::copy_if(in.begin(), in.end(), std::back_inserter(out), [c](auto x) {
                     return std::find(c.begin(), c.end(), x) == c.end();
                   });
      return out;
    }

    std::string removeCharacter(const std::string& in, char c)
    {
      std::string out;
      std::copy_if(in.begin(), in.end(), std::back_inserter(out), [c](auto x) {
          return x != c;
      });
      return out;
    }

    std::string removeSpaces(const std::string& in)
    {
      return removeCharacter(in, ' ');
    }
  }
}
