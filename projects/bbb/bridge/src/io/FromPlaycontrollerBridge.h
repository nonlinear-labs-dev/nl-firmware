#pragma once

#include <io/Bridge.h>

class FromPlaycontrollerBridge : public Bridge
{
 public:
  FromPlaycontrollerBridge();
  void sendRibbonPosition(bool m_upperRibon, double value);

 private:
  void transmit(const Receiver::tMessage &msg) override;
};
