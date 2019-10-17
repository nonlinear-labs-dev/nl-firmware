#include <proxies/hwui/panel-unit/boled/parameter-screens/MonoModeParameterLayout.h>
#include "MonoParameter.h"

DFBLayout *MonoParameter::createLayout(FocusAndMode focusAndMode) const {
  return new MonoModeParameterLayout();
}

MonoParameter::MonoParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                             tControlPositionValue def, tControlPositionValue coarseDenominator,
                             tControlPositionValue fineDenominator)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
{
}
