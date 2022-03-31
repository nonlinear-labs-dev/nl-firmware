#pragma once

#include <string>
#include "nltools/logging/Log.h"

namespace Environment
{
  void printStackTrace(int i);
  std::string getStackTrace(const std::string& prefix);
  void printbacktrace(nltools::Log::Level l, int maxFrames = 500);
  void setupLocale();
}

std::ostream& wirteIntToStream(std::ostream& stream, int i);
std::string to_string(double d);
