#include "UnmodulateableUnisonParameter.h"



DFBLayout *UnmodulateableUnisonParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Select:
    default:
      return new UnmodulateableUnisonParameterLayout();
    case UIMode::Edit:
      return new UnmodulateableParameterEditLayout2();
    case UIMode::Info:
      return new ParameterInfoLayout();
  }
}
