#pragma once
#include "MonoParameter.h"

class MonoModeEnableParameter : public Parameter
{
 public:
  MonoModeEnableParameter(ParameterGroup *group, ParameterId id);

  Glib::ustring getDisplayString() const override;
  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
};
