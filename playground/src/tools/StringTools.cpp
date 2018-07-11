#include "StringTools.h"
#include <sstream>

namespace StringTools {
    std::vector<std::string> splitStringOnAnyDelimiter(const std::string &s, char delimiter) {
      std::vector<std::string> strings;
      std::string token;
      std::istringstream tokenStream(s);
      while (
              std::getline(tokenStream, token, delimiter
              )) {
        strings.
                push_back(token);
      }
      return
              strings;
    }
}