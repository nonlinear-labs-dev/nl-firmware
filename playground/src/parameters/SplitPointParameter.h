#pragma once
#include "ModulateableParameter.h"

class SplitPointParameter : public ModulateableParameter
{
 public:
  SplitPointParameter(ParameterGroup *group, const ParameterId& id);
  std::string getDisplayValue(VoiceGroup vg) const;

  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;

  Glib::ustring getGroupAndParameterName() const override;

  Glib::ustring getDisplayString() const override;
};
