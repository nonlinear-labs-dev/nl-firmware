#include <proxies/hwui/panel-unit/boled/parameter-screens/MonoModeParameterLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include "MonoParameter.h"

DFBLayout *MonoParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Select:
    default:
      return new MonoModeParameterLayout();
    case UIMode::Edit:
      return new UnmodulateableParameterEditLayout2();
    case UIMode::Info:
      return new ParameterInfoLayout();
  }
}

MonoParameter::MonoParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                             tControlPositionValue def, tControlPositionValue coarseDenominator,
                             tControlPositionValue fineDenominator)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
{
}
