#include <proxies/hwui/panel-unit/boled/parameter-screens/DualVoiceGroupMasterAndSplitPointLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

#include <utility>
#include "VoiceGroupMasterParameter.h"

VoiceGroupMasterParameter::VoiceGroupMasterParameter(ParameterGroup *group, const ParameterId &id,
                                                     const ScaleConverter *scaling, tControlPositionValue def,
                                                     tControlPositionValue coarseDenominator,
                                                     tControlPositionValue fineDenominator, std::string shortName,
                                                     std::string longName, VoiceGroup vg)
    : ModulateableParameter(group, id, scaling, def, coarseDenominator, fineDenominator)
    , m_longname{ std::move(longName) }
    , m_shortname{ std::move(shortName) }
    , m_vg{ vg }
{
}

Glib::ustring VoiceGroupMasterParameter::getLongName() const
{
  return m_longname;
}

Glib::ustring VoiceGroupMasterParameter::getShortName() const
{
  return m_shortname;
}

Glib::ustring VoiceGroupMasterParameter::getGroupAndParameterName() const
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
      return new ModulateableParameterEditLayout2();
    case UIMode::Select:
    default:
      return new DualVoiceGroupMasterAndSplitPointLayout();
  }
}