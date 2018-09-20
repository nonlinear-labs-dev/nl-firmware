#pragma once

#include <io/Bridge.h>
#include <tools/Throttler.h>
#include <list>

class FromEncoderBridge : public Bridge
{
 public:
  FromEncoderBridge();
  virtual ~FromEncoderBridge();

  void sendRotary(int8_t inc);

 private:
  void scheduleSimpleEvent();
  void scheduleTimestampedEvent();

  int m_accumulated = 0;
  std::chrono::system_clock::time_point m_firstPendingEventTime;
  Throttler m_throttler;
  static Domain getDomain();
};
