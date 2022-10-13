#pragma once
#include "proxies/hwui/controls/DotSlider.h"

class MacroControlSliderForCurrentModulationRouter : public DotSlider
{
 public:
  MacroControlSliderForCurrentModulationRouter(const Rect& pos);
  void subscribeToMacroFromSelected(Parameter* p);
};
