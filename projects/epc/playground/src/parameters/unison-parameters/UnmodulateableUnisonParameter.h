#pragma once

#include <parameters/Parameter.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/UnisonParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/UnmodulatebaleParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

class UnmodulateableUnisonParameter : public Parameter
{
 public:
  template <typename... tArgs>
  explicit UnmodulateableUnisonParameter(tArgs... args)
      : Parameter(args...)
  {
  }

  Layout *createLayout(FocusAndMode focusAndMode) const override;
};

class ModulateableUnisonParameter : public ModulateableParameter
{
 public:
  template <typename... tArgs>
   explicit ModulateableUnisonParameter(tArgs... args)
    : ModulateableParameter(args...)
   {
   }

   Layout *createLayout(FocusAndMode focusAndMode) const override;
};
