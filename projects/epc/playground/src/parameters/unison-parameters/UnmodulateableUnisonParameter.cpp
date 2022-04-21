#include "UnmodulateableUnisonParameter.h"

Layout *UnmodulateableUnisonParameter::createLayout(FocusAndMode focusAndMode) const
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

Layout *ModulateableUnisonParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Select:
    default:
      return new ModulateableUnisonParameterLayout();
    case UIMode::Edit:
      return new ModulateableParameterEditLayout2();
    case UIMode::Info:
      return new ParameterInfoLayout();
  }
}