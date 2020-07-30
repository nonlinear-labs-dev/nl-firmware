#pragma once

#include <io/Bridge.h>

namespace nltools
{
  namespace msg
  {
    struct PlaycontrollerMessage;
  }
}

class ToPlaycontrollerBridge : public Bridge
{
  using super = Bridge;

 public:
  ToPlaycontrollerBridge();

 private:
  void onMessage(const nltools::msg::PlaycontrollerMessage &msg);
};
