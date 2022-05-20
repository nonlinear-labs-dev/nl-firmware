#include "UnmodulateableMonoParameter.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/MonoModeParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/UnmodulatebaleParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

UnmodulateableMonoParameter::UnmodulateableMonoParameter(ParameterGroup* group, const ParameterId& id,
                                                         const ScaleConverter* scaling)
    : Parameter { group, id, scaling }
{
}

Layout *UnmodulateableMonoParameter::createLayout(FocusAndMode focusAndMode) const
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
