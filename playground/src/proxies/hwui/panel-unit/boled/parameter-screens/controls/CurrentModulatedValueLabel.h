#pragma once

#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationBoundLabel.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include "Defaultable.h"

class MacroControlParameter;
class ModulateableParameter;

class CurrentModulatedValueLabel : public ModulationBoundLabel, public RotaryEncoder::Receiver, public Defaultable
{
    typedef ModulationBoundLabel super;

  public:
  explicit CurrentModulatedValueLabel (const Rect &r);
  ~CurrentModulatedValueLabel () override;

    bool onRotary (int inc, ButtonModifiers modifiers) override;
    void setDefault() override;

  protected:
    void updateText(MacroControlParameter *mcParam, ModulateableParameter *modulatedParam) override;
};

