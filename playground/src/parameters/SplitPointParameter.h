#pragma once
#include "ModulateableParameter.h"

class SplitPointParameter : public ModulateableParameter
{
 public:
  SplitPointParameter(ParameterGroup *group, const ParameterId &id);
  std::string getDisplayValue(VoiceGroup vg) const;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
  Glib::ustring getDisplayString() const override;
};
