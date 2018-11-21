#pragma once

#include "ModulationBoundControl.h"

class UpperModulationBoundControl : public ModulationBoundControl
{
  typedef ModulationBoundControl super;

 public:
  explicit UpperModulationBoundControl(const Rect &r);
  bool onRotary(int inc, ButtonModifiers modifiers) override;
};
