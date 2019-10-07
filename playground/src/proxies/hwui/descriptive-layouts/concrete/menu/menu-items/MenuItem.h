#pragma once
#include <proxies/hwui/controls/ControlWithChildren.h>

class MenuItem : public ControlWithChildren {
public:
  MenuItem();

  virtual bool canEnter() = 0;
  virtual void doAction() = 0;
};
