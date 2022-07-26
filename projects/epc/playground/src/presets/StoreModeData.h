#pragma once

#include <stddef.h>
#include <limits>

struct StoreModeData
{
 public:
  StoreModeData();
  constexpr static int invalid = std::numeric_limits<int>::max();
  int bankPos = invalid;
  int presetPos = invalid;
};
