#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include "Defaultable.h"

class UpperModulationBoundControl : public ControlWithChildren, public RotaryEncoder::Receiver, public Defaultable
{
    typedef ControlWithChildren super;
  public:
    explicit UpperModulationBoundControl (const Rect &r);
    ~UpperModulationBoundControl () override;
    bool onRotary (int inc, ButtonModifiers modifiers) override;
    void setDefault() override;
};

