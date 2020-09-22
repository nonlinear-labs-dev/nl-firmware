#pragma once
#include "ModulateableParameterWithUnusualModUnit.h"

class SplitPointParameter : public ModulateableParameterWithUnusualModUnit
{
 public:
  SplitPointParameter(ParameterGroup *group, const ParameterId &id);

  Layout *createLayout(FocusAndMode focusAndMode) const override;
  Glib::ustring getDisplayString() const override;
  Glib::ustring getDisplayString(tControlPositionValue cp) const override;
  Glib::ustring stringizeModulationAmount(tControlPositionValue amount) const override;

 protected:
  Glib::ustring modulationValueToDisplayString(tControlPositionValue v) const override;
};
