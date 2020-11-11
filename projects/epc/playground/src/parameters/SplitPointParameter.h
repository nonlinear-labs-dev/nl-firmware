#pragma once
#include <tools/RecursionGuard.h>
#include "ModulateableParameterWithUnusualModUnit.h"

class Setting;
class Preset;

class SplitPointParameter : public ModulateableParameterWithUnusualModUnit, public sigc::trackable
{
 public:
  SplitPointParameter(ParameterGroup* group, const ParameterId& id);

  Layout* createLayout(FocusAndMode focusAndMode) const override;
  Glib::ustring stringizeModulationAmount(tControlPositionValue amount) const override;
  void setModulationAmount(UNDO::Transaction* transaction, const tDisplayValue& amount) override;
  void setModulationAmountFromSibling(UNDO::Transaction* transaction, const tDisplayValue& amount);
  void setModulationSource(UNDO::Transaction* transaction, MacroControls src) override;
  void setModulationSourceFromSibling(UNDO::Transaction* transaction, MacroControls src);
  SplitPointParameter* getSibling() const;
  bool hasOverlap();
  bool inDefaultSplitBehaviour() const;

 protected:
  void onSyncSettingChanged(const Setting* s);
  void onSyncSettingChangedWithTransaction(const Setting* s, UNDO::Transaction* transaction);

  void onStoreHappened(UNDO::Transaction* transaction, Preset* newPreset);

  void setCpValue(UNDO::Transaction* transaction, Initiator initiator, tControlPositionValue value,
                  bool dosendToPlaycontroller) override;
  bool inModAmountSet = false;
  bool inModSrcSet = false;
  void preventNegativeOverlap(UNDO::Transaction* transaction, tControlPositionValue value, bool dosendToPlaycontroller);
  bool isAtExtremes(tControlPositionValue value);

  void clampToExtremes(UNDO::Transaction* transaction, bool dosendToPlaycontroller);
  RecursionGuard m_settingGuard;
};
