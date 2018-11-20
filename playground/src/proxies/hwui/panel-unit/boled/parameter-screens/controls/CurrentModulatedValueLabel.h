#pragma once

#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationBoundLabel.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include <proxies/hwui/panel-unit/ButtonReceiver.h>

class MacroControlParameter;
class ModulateableParameter;

class CurrentModulatedValueLabel : public ModulationBoundLabel, public RotaryEncoder::Receiver, public ButtonReceiver
{
  typedef ModulationBoundLabel super;

 public:
  explicit CurrentModulatedValueLabel(const Rect &r);

  bool onRotary(int inc, ButtonModifiers modifiers) override;
  bool onButton(int i, bool down, ButtonModifiers modifiers) override;

 protected:
  void updateText(MacroControlParameter *mcParam, ModulateableParameter *modulatedParam) override;
};
