#pragma once

#include <stddef.h>
#include <limits>

struct StoreModeData
{
 public:
  StoreModeData();
  constexpr static size_t invalid = std::numeric_limits<size_t>::max();
  size_t bankPos = invalid;
  size_t presetPos = invalid;
};
