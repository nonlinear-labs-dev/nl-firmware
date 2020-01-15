#include <nltools/enums/EnumTools.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace EnumTools
{
  std::vector<std::string> parseDef(const std::string &def)
  {
    std::vector<std::string> strs;
    boost::split(strs, def, boost::is_any_of(","));

    for(auto &key : strs)
    {
      key = boost::trim_copy(key);
      auto pos = key.find_first_of(" =,;\n\r");
      if(pos != std::string::npos)
        key = key.substr(0, pos);
    }
    return strs;
  }
}
