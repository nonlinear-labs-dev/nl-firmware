#pragma once
#include "Parameter.h"

class SplitPointParameter : public Parameter
{
 public:
  SplitPointParameter(ParameterGroup *group, ParameterId id);
  std::string getDisplayValue(VoiceGroup vg) const;
  static void registerTests();

  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;

  Glib::ustring getGroupAndParameterName() const override;

  Glib::ustring getDisplayString() const override;
};
