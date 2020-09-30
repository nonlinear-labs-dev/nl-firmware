#pragma once
#include "ModulateableParameterWithUnusualModUnit.h"

class SplitPointParameter : public ModulateableParameterWithUnusualModUnit
{
 public:
  SplitPointParameter(ParameterGroup* group, const ParameterId& id);

  Layout* createLayout(FocusAndMode focusAndMode) const override;
  Glib::ustring stringizeModulationAmount(tControlPositionValue amount) const override;

 protected:
  SplitPointParameter* getSibling();

  void setCpValue(UNDO::Transaction* transaction, Initiator initiator, tControlPositionValue value,
                  bool dosendToPlaycontroller) override;
};
