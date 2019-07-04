#pragma once

#include "playground.h"
#include "proxies/hwui/HWUIEnums.h"
#include <nltools/threading/Throttler.h>
#include <chrono>
#include <io/network/WebSocketSession.h>

class RotaryEncoder
{
 public:
  class Receiver
  {
   public:
    virtual bool onRotary(int inc, ButtonModifiers modifiers) = 0;
  };

  typedef signed char tIncrement;

  RotaryEncoder();
  virtual ~RotaryEncoder();

  void fake(tIncrement amount);
  int64_t resetOldestPendingTimestamp();
  sigc::connection onRotaryChanged(sigc::slot<void, tIncrement> slot);

  static void registerTests();

 private:
  void onMessage(WebSocketSession::tMessage msg);
  void open();
  tIncrement speedUp(tIncrement inc);
  void applyIncrement(tIncrement currentInc);
  void onTimeStampedMessage(WebSocketSession::tMessage msg);

  Signal<void, tIncrement> m_signalRotaryChanged;
  Throttler m_throttler;
  int m_accumulatedIncs = 0;

  int64_t m_oldestPendingTimestamp = 0;

  sigc::connection m_stress;
};
