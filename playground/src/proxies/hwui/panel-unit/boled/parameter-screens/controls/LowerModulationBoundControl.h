#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include "Defaultable.h"

class LowerModulationBoundControl : public ControlWithChildren, public RotaryEncoder::Receiver, public Defaultable
{
    typedef ControlWithChildren super;

  public:
  explicit LowerModulationBoundControl (const Rect &r);
  ~LowerModulationBoundControl () override;

    bool onRotary (int inc, ButtonModifiers modifiers) override;
    void setDefault() override;

  private:
    void onSelectionChanged (Parameter *, Parameter *newParam);
    void onParameterChanged (const Parameter *p);

    Control *m_dummyButton = nullptr;
    connection m_paramChanged;

  double calcDominator(const ButtonModifiers &modifiers, const ModulateableParameter *modulatedParam) const;
};

