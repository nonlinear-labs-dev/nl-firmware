#include "PerformanceTimer.h"
#include <chrono>
#include <iostream>

int PerformanceTimer::s_indent = 0;

void PerformanceTimer::printCurrentTime(const char *name)
{
  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  printCurrentTime(name, ms);
}

void PerformanceTimer::printCurrentTime(const char *name, int64_t ms)
{
  /* auto s = ms / 1000;
  ms -= s * 1000;

  auto m = s / 60;
  s -= m * 60;

  auto h = m / 60;
  m -= h * 60;

  auto d = h / 24;
  h -= d * 24;

  auto y = d / 365;
  d -= y * 360;

  char txt[256];
  sprintf(txt, "%ld:%ld:%02ld:%02ld:%02ld:%03ld", y, d, h, m, s, ms);*/

  std::cout << "Performance time stamp at " << name << ": " << ms << std::endl;
}
