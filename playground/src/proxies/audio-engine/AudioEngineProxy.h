#pragma once

#include "playground.h"

namespace UNDO
{
  class Transaction;
}

class AudioEngineProxy
{
 public:
  void sendParameter(uint16_t id, tControlPositionValue value);
  void sendEditBuffer();
  void toggleSuppressParameterChanges(UNDO::Transaction *transaction);

 private:
  bool m_suppressParamChanges = false;
};
