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
  void init(Settings &s);

 public:
  void undoableSetPedalMode(UNDO::Transaction *transaction, PedalModes mode, Initiator initiator);
  void undoableSetPedalMode(UNDO::Transaction *transaction, const Glib::ustring &mode, Initiator initiator);
  PedalModes getPedalMode() const;
  ReturnMode getReturnMode() const override;
  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other) override;
  void copyTo(UNDO::Transaction *transaction, PresetParameter *other) const override;
  void loadDefault(UNDO::Transaction *transaction, Defaults mode) override;

  PedalType *getAssociatedPedalTypeSetting() const;
  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
  Glib::ustring getLongNameWithoutSuffix() const;
  Glib::ustring getShortNameWithoutSuffix() const;

 protected:
  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;
  bool shouldWriteDocProperties(tUpdateID knownRevision) const override;

  bool hasBehavior() const override;
  Glib::ustring getCurrentBehavior() const override;
  void undoableStepBehavior(UNDO::Transaction *transaction, int direction) override;

  size_t getHash() const override;
  tControlPositionValue getDefValueAccordingToMode() const override;

 private:
  void onPedalTypeChanged(const Setting *s);
  const ScaleConverter *createScaleConverter() const;
  void sendModeToPlaycontroller() const;
  void setRoutersModeAccordingToReturnMode();
  bool isLocalEnabled() const override;
  bool isHardwareDisabled() const;

  PedalModes m_mode = PedalModes::STAY;
  tUpdateID m_updateIdWhenModeChanged = 0;
  PedalType *m_setting = nullptr;
};
