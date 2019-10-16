#include <proxies/hwui/panel-unit/boled/parameter-screens/DualSpecialParameterScreen.h>
#include "VGMasterParameter.h"

VGMasterParameter::VGMasterParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                                     tControlPositionValue def, tControlPositionValue coarseDenominator,
                                     tControlPositionValue fineDenominator)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
{
}

ustring VGMasterParameter::getLongName() const
{
  return ParameterDB::get().getLongName(getID() - 11000);
}

ustring VGMasterParameter::getShortName() const
{
  return ParameterDB::get().getShortName(getID() - 11000);
}

ustring VGMasterParameter::getGroupAndParameterName() const
{
  return "VGM - " + getLongName();
}

DFBLayout *VGMasterParameter::createLayout(FocusAndMode focusAndMode) const
{
  return new DualSpecialParameterScreen();
}
