#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include <proxies/hwui/panel-unit/ButtonReceiver.h>

class UpperModulationBoundControl : public ControlWithChildren, public RotaryEncoder::Receiver, public ButtonReceiver
{
  typedef ControlWithChildren super;

 public:
  explicit UpperModulationBoundControl(const Rect &r);
  bool onRotary(int inc, ButtonModifiers modifiers) override;
  bool onButton (int i, bool down, ButtonModifiers modifiers) override;
};
