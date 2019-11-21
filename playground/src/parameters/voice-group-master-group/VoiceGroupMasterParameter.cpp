#include <proxies/hwui/panel-unit/boled/parameter-screens/DualVoiceGroupMasterAndSplitPointLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include "VoiceGroupMasterParameter.h"

VoiceGroupMasterParameter::VoiceGroupMasterParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                                                     tControlPositionValue def, tControlPositionValue coarseDenominator,
                                                     tControlPositionValue fineDenominator,
                                                     const std::string &shortName, const std::string &longName,
                                                     VoiceGroup vg)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
    , m_longname{ longName }
    , m_shortname{ shortName }
    , m_vg{ vg }
{
}

ustring VoiceGroupMasterParameter::getLongName() const
{
  return m_longname;
}

ustring VoiceGroupMasterParameter::getShortName() const
{
  return m_shortname;
}

ustring VoiceGroupMasterParameter::getGroupAndParameterName() const
{
  return getParentGroup()->getShortName() + getLongName();
}

DFBLayout *VoiceGroupMasterParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new UnmodulateableParameterEditLayout2();
    case UIMode::Select:
    default:
      return new DualVoiceGroupMasterAndSplitPointLayout();
  }
}