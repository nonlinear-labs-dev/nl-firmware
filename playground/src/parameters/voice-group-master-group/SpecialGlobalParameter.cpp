#include <proxies/hwui/panel-unit/boled/parameter-screens/DualSpecialParameterScreen.h>
#include "SpecialGlobalParameter.h"

SpecialGlobalParameter::SpecialGlobalParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                                               tControlPositionValue def, tControlPositionValue coarseDenominator,
                                               tControlPositionValue fineDenominator, const std::string &shortName,
                                               const std::string &longName)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
    , m_longname{ longName }
    , m_shortname{ shortName }
{
}

ustring SpecialGlobalParameter::getLongName() const
{
  return m_longname;
}

ustring SpecialGlobalParameter::getShortName() const
{
  return m_shortname;
}

ustring SpecialGlobalParameter::getGroupAndParameterName() const
{
  return getParentGroup()->getShortName() + getLongName();
}

DFBLayout *SpecialGlobalParameter::createLayout(FocusAndMode focusAndMode) const
{
  return new DualSpecialParameterScreen();
}

SpecialPolyParameter::SpecialPolyParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                                           tControlPositionValue def, tControlPositionValue coarseDenominator,
                                           tControlPositionValue fineDenominator, const std::string &shortName,
                                           const std::string &longName, VoiceGroup vg)
    : SpecialGlobalParameter(group, id, scaling, def, coarseDenominator, fineDenominator, shortName, longName)
{
  m_vg = vg;
}

DFBLayout *SpecialPolyParameter::createLayout(FocusAndMode focusAndMode) const
{
  return new UnmodulateableParameterSelectLayout2();
}