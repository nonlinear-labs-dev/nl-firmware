#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableDualVoiceGroupMasterAndSplitPointLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <utility>
#include "VoiceGroupMasterModulateableParameter.h"
#include <groups/ParameterGroup.h>

VoiceGroupMasterModulateableParameter::VoiceGroupMasterModulateableParameter(ParameterGroup *group,
                                                                             const ParameterId &id,
                                                                             const ScaleConverter *scaling)
    : ModulateableParameter(group, id, scaling)
{
}

Layout *VoiceGroupMasterModulateableParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new ModulateableParameterEditLayout2();
    case UIMode::Select:
    default:
      return new ModulateableDualVoiceGroupMasterAndSplitPointLayout();
  }
}

Layout *VoiceGroupMasterUnmodulateableParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new UnmodulateableParameterEditLayout2();
    case UIMode::Select:
    default:
      return new UnmodulateableDualVoiceGroupMasterAndSplitPointLayout();
  }
}
