#pragma once

#include "PhysicalControlParameter.h"

class PedalType;

class PedalParameter : public PhysicalControlParameter
{
 private:
  typedef PhysicalControlParameter super;

 public:
  using super::super;
  void onLocalEnableChanged(bool localEnableState) override;

 public:
  void undoableSetPedalMode(UNDO::Transaction *transaction, PedalModes mode, Initiator initiator);
  void undoableSetPedalMode(UNDO::Transaction *transaction, const Glib::ustring &mode, Initiator initiator);
  PedalModes getPedalMode() const;
  ReturnMode getReturnMode() const override;
  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other) override;
  void copyTo(UNDO::Transaction *transaction, PresetParameter *other) const override;
  void loadDefault(UNDO::Transaction *transaction, Defaults mode) override;

  PedalType *getAssociatedPedalTypeSetting() const;
  QuantizedValue &getValue() override;
  void setIndirect(UNDO::Transaction *transaction, const tControlPositionValue &value) override;
  void loadFromPreset(UNDO::Transaction *transaction, const tControlPositionValue &value) override;

 protected:
  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;
  bool shouldWriteDocProperties(tUpdateID knownRevision) const override;

  bool hasBehavior() const override;
  Glib::ustring getCurrentBehavior() const override;
  void undoableStepBehavior(UNDO::Transaction *transaction, int direction) override;

  Layout *createLayout(FocusAndMode focusAndMode) const override;
  size_t getHash() const override;
  tControlPositionValue getDefValueAccordingToMode() const override;
  void setCpValue(UNDO::Transaction *transaction, Initiator initiator, tControlPositionValue value,
                  bool dosendToPlaycontroller) override;
  void onValueChanged(Initiator initiator, tControlPositionValue oldValue, tControlPositionValue newValue) override;

 private:
  const ScaleConverter *createScaleConverter() const;
  void sendModeToPlaycontroller() const;
  void setRoutersModeAccordingToReturnMode();
  bool isLocalEnabled() const override;

  PedalModes m_mode = PedalModes::STAY;
  tUpdateID m_updateIdWhenModeChanged = 0;
};
