#pragma once

#include "Application.h"
#include "testing/TestDriver.h"
#include <cstdlib>
#include <execinfo.h>
#include <iostream>
#include <chrono>
#include "device-settings/DebugLevel.h"
#include "profiling/Profiler.h"
#include "Options.h"
#include <nltools/StringTools.h>

namespace Environment
{
  void printStackTrace(int i);
  std::string getStackTrace(const std::string& prefix);
  void setupLocale();
}
