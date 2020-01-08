#pragma once

#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/UnisonParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

class ModulateableUnisonParameterWithUnusualModUnit : public ModulateableParameterWithUnusualModUnit
{
 public:
  template <typename... tArgs>
  ModulateableUnisonParameterWithUnusualModUnit(tArgs... args)
      : ModulateableParameterWithUnusualModUnit(args...)
  {
  }

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
};