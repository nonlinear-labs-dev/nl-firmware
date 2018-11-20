#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include <proxies/hwui/panel-unit/ButtonReceiver.h>

class LowerModulationBoundControl : public ControlWithChildren, public RotaryEncoder::Receiver, public ButtonReceiver
{
  typedef ControlWithChildren super;

 public:
  LowerModulationBoundControl(const Rect &r);
  virtual ~LowerModulationBoundControl();

  bool onRotary(int inc, ButtonModifiers modifiers) override;
  bool onButton(int i, bool down, ButtonModifiers modifiers) override;

 private:
  void onSelectionChanged(Parameter *, Parameter *newParam);
  void onParameterChanged(const Parameter *p);

  Control *m_dummyButton = nullptr;
  connection m_paramChanged;
};
