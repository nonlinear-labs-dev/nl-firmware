#pragma once

#include "LED.h"
#include "nltools/threading/Throttler.h"

class FourStateLED : public LED
{
 public:
  FourStateLED();
  ~FourStateLED() override;

  enum State
  {
    Invalid = -1,
    Off = 0,
    Dark = 1,
    Medium = 2,
    Bright = 3
  };

  void setState(State state);
  [[nodiscard]] State getState() const;
  void syncHWUI();

 private:
  Throttler m_syncThrottler;
  State m_state = State::Invalid;
};
