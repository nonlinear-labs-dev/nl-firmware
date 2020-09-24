#pragma once

#include <stdint.h>
#include <nltools/GlibFwd.h>

enum class Domain : uint8_t
{
  Playcontroller = 1,
};

void printCurrentTime(const char *s);
void printCurrentTime(const char *s, int64_t ms);
