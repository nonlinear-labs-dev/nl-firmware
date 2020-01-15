#pragma once

#include <string>

namespace Environment
{
  void printStackTrace(int i);
  std::string getStackTrace(const std::string& prefix);
  void setupLocale();
}

std::ostream& wirteIntToStream(std::ostream& stream, int i);
std::string to_string(double d);
