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

ustring VGMasterParameter::getGroupAndParameterName() const
{
  return "VGM - " + getLongName();
}

DFBLayout *VGMasterParameter::createLayout(FocusAndMode focusAndMode) const
{
  return Parameter::createLayout(focusAndMode);
}
