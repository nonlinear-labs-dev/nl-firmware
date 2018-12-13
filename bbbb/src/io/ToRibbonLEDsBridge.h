#pragma once

#include <io/Bridge.h>

class ToRibbonLEDsBridge : public Bridge
{
  using super = Bridge;

 public:
  ToRibbonLEDsBridge();
  virtual ~ToRibbonLEDsBridge();
};
