#pragma once

#include <io/Bridge.h>
#include <tools/Throttler.h>

class FromEncoderBridge : public Bridge
{
  public:
    FromEncoderBridge();
    virtual ~FromEncoderBridge();

    void sendRotary(int8_t inc);

   private:
    int m_accumulated = 0;
    Throttler m_throttler;
};

