#pragma once

#include "playground.h"

class AudioEngineProxy
{
 public:
  void sendParameter(uint16_t id, tControlPositionValue value);
  void sendEditBuffer();
};
