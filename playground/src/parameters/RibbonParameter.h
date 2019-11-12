#pragma once

#include "../http/UpdateDocumentContributor.h"
#include "../libundo/undo/Scope.h"
#include "../playground.h"
#include "../proxies/hwui/HWUIEnums.h"
#include "../xml/Writer.h"
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
  void loadDefault(UNDO::Transaction *transaction) override;

  void boundToMacroControl(tControlPositionValue v);

  virtual DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
  virtual void loadFromPreset(UNDO::Transaction *transaction, const tControlPositionValue &value) override;

 protected:
  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;

  virtual void onPresetSentToLpc() const override;
  virtual bool shouldWriteDocProperties(tUpdateID knownRevision) const override;
  virtual bool hasBehavior() const override;
  virtual Glib::ustring getCurrentBehavior() const override;
  virtual void undoableStepBehavior(UNDO::Transaction *transaction, int direction) override;
  virtual size_t getHash() const override;

 private:
  void ensureExclusiveRoutingIfNeeded();
  void sendModeToLpc() const;
  const ScaleConverter *createScaleConverter() const;
  tControlPositionValue getDefaultValueAccordingToMode() const;
  void setupScalingAndDefaultValue();

  RibbonTouchBehaviour m_touchBehaviour = RibbonTouchBehaviour::ABSOLUTE;
  RibbonReturnMode m_returnMode = RibbonReturnMode::STAY;
  tUpdateID m_updateIdWhenModeChanged = 0;
};
