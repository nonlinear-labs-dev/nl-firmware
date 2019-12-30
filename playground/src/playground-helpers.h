#pragma once

#include <string>

namespace Environment
{
  void printStackTrace(int i);
  std::string getStackTrace(const std::string& prefix);
  void setupLocale();
}
