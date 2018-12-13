#pragma once

#include "PhysicalControlParameter.h"

class PedalType;

class PedalParameter : public PhysicalControlParameter
{
 private:
  typedef PhysicalControlParameter super;

 public:
  using super::super;

  void undoableSetPedalMode(UNDO::Transaction *transaction, PedalModes mode);
  void undoableSetPedalMode(UNDO::Transaction *transaction, const Glib::ustring &mode);
  void undoableIncPedalMode(UNDO::Transaction *transaction);
  PedalModes getPedalMode() const;
  ReturnMode getReturnMode() const override;
  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other) override;
  void copyTo(UNDO::Transaction *transaction, PresetParameter *other) const override;
  void loadDefault(UNDO::Transaction *transaction) override;

  shared_ptr<PedalType> getAssociatedPedalTypeSetting() const;

 protected:
  virtual void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;
  void writeDifferences(Writer &writer, Parameter *other) const override;
  virtual void onPresetSentToLpc() const override;
  virtual bool shouldWriteDocProperties(tUpdateID knownRevision) const override;

  virtual bool hasBehavior() const override;
  virtual Glib::ustring getCurrentBehavior() const override;
  virtual void undoableStepBehavior(UNDO::Transaction *transaction, int direction) override;

  virtual DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
  virtual size_t getHash() const override;

 private:
  const ScaleConverter *createScaleConverter() const;
  uint16_t mapParameterIdToLPCSetting() const;
  void sendModeToLpc() const;
  tControlPositionValue getDefValueAccordingToMode() const;
  void setRoutersModeAccordingToReturnMode();

  PedalModes m_mode = PedalModes::STAY;
  tUpdateID m_updateIdWhenModeChanged = 0;
};
