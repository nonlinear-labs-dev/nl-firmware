#pragma once

#include "proxies/hwui/controls/Control.h"

class DebugLed : public Control
{
 private:
  typedef Control super;

 public:
  DebugLed(const Rect& pos, int led);

  void setLedState(bool onOrOff);
  bool redraw(FrameBuffer& fb) override;
  int getId() const;

 private:
  int m_ledId;
  bool m_onOrOff;
};
