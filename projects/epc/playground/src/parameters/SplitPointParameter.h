#pragma once
#include "ModulateableParameterWithUnusualModUnit.h"

class SplitPointSyncParameters;

class SplitPointParameter : public ModulateableParameterWithUnusualModUnit
{
 public:
  SplitPointParameter(ParameterGroup* group, const ParameterId& id);

  Layout* createLayout(FocusAndMode focusAndMode) const override;
  Glib::ustring getDisplayString() const override;
  Glib::ustring getDisplayString(tControlPositionValue cp) const override;
  Glib::ustring stringizeModulationAmount(tControlPositionValue amount) const override;

 protected:
  SplitPointParameter* getSibling();
  Glib::ustring modulationValueToDisplayString(tControlPositionValue v) const override;
  void setCpValue(UNDO::Transaction* transaction, Initiator initiator, tControlPositionValue value,
                  bool dosendToPlaycontroller) override;

 private:
  SplitPointSyncParameters* m_syncSetting;
};
