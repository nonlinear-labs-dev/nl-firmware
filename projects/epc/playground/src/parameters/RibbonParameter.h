#pragma once

#include <playground.h>
#include <proxies/hwui/HWUIEnums.h>
#include "PhysicalControlParameter.h"

class RibbonParameter : public PhysicalControlParameter
{
 private:
  typedef PhysicalControlParameter super;

 public:
  using super::super;

  void undoableSetRibbonTouchBehaviour(UNDO::Transaction *transaction, RibbonTouchBehaviour mode);
  void undoableSetRibbonTouchBehaviour(UNDO::Transaction *transaction, const Glib::ustring &mode);
  void undoableIncRibbonTouchBehaviour(UNDO::Transaction *transaction);
  void undoableSetRibbonReturnMode(UNDO::Transaction *transaction, RibbonReturnMode mode);
  void undoableSetRibbonReturnMode(UNDO::Transaction *transaction, const Glib::ustring &mode);

  RibbonTouchBehaviour getRibbonTouchBehaviour() const;
  RibbonReturnMode getRibbonReturnMode() const;

  ReturnMode getReturnMode() const override;
  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other) override;
  void copyTo(UNDO::Transaction *transaction, PresetParameter *other) const override;
  void loadDefault(UNDO::Transaction *transaction, Defaults mode) override;

  void boundToMacroControl(tControlPositionValue v);

  Layout *createLayout(FocusAndMode focusAndMode) const override;

  void sendModeToPlaycontroller() const;
  tControlPositionValue getDefValueAccordingToMode() const override;
  bool isLocalEnabled() const override;
  void onLocalEnableChanged(bool localEnableState) override;
  void setCPFromSetting(UNDO::Transaction *transaction, const tControlPositionValue &cpValue) override;
  void loadFromPreset(UNDO::Transaction *transaction, const tControlPositionValue &value) override;
  void setIndirect(UNDO::Transaction *transaction, const tControlPositionValue &value) override;

 protected:
  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;

  bool shouldWriteDocProperties(tUpdateID knownRevision) const override;
  bool hasBehavior() const override;

 protected:
  Glib::ustring getCurrentBehavior() const override;
  void undoableStepBehavior(UNDO::Transaction *transaction, int direction) override;
  size_t getHash() const override;
  void sendToPlaycontroller() const override;
  void setCpValue(UNDO::Transaction *transaction, Initiator initiator, tControlPositionValue value,
                  bool dosendToPlaycontroller) override;

 private:
  void ensureExclusiveRoutingIfNeeded();
  const ScaleConverter *createScaleConverter() const;
  void setupScalingAndDefaultValue(bool defaultValue);
  std::list<ModulationRoutingParameter*> getRoutingParameters() const;
  void undoableSetHWAmountsForReturnToCenterMode(UNDO::Transaction *transaction, const RibbonReturnMode &mode) const;

  RibbonTouchBehaviour m_touchBehaviour = RibbonTouchBehaviour::ABSOLUTE;
  RibbonReturnMode m_returnMode = RibbonReturnMode::STAY;
  tUpdateID m_updateIdWhenModeChanged = 0;
};
