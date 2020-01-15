#pragma once

#include <io/Bridge.h>

class FromEncoderBridge : public Bridge
{
 public:
  FromEncoderBridge();
  void sendRotary(int8_t inc);

 private:
  void transmit(const Receiver::tMessage &msg) override;
};
