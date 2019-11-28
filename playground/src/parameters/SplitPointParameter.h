#pragma once
#include "Parameter.h"

class SplitPointParameter : public Parameter
{
 public:
  SplitPointParameter(ParameterGroup *group, ParameterId id);
  std::string getDisplayValue(VoiceGroup vg) const;
  static void registerTests();

  ustring getLongName() const override;
  ustring getShortName() const override;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;

  ustring getGroupAndParameterName() const override;

  ustring getDisplayString() const override;
};
