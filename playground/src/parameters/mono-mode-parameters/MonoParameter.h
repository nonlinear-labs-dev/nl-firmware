#pragma once
#include <parameters/Parameter.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/MonoModeParameterLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

template <typename tBaseParameter> class MonoParameter : public tBaseParameter
{
 public:
  MonoParameter(ParameterGroup *group, const ParameterId &id, const ScaleConverter *scaling, tControlPositionValue def,
                tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator)
      : tBaseParameter(group, id, scaling, def, coarseDenominator, fineDenominator)
  {
  }

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override
  {
    switch(focusAndMode.mode)
    {
      case UIMode::Select:
      default:
        if constexpr(std::is_base_of<ModulateableParameter, tBaseParameter>::value)
        {
          return new MonoModeModulateableParameterLayout();
        }
        else
        {
          return new MonoModeParameterLayout();
        }
      case UIMode::Edit:
        if constexpr(std::is_base_of<ModulateableParameter, tBaseParameter>::value)
        {
          return new ModulateableParameterEditLayout2();
        }
        else
        {
          return new UnmodulateableParameterEditLayout2();
        }
      case UIMode::Info:
        return new ParameterInfoLayout();
    }
  }
};
