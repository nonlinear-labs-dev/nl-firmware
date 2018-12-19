#pragma once

#include "Parameter.h"
#include <proxies/hwui/HWUIEnums.h>
#include <set>

class ModulateableParameter;

class MacroControlParameter : public Parameter
{
  typedef Parameter super;

 public:
  MacroControlParameter(ParameterGroup *group, uint16_t id);
  ~MacroControlParameter() override;

  typedef set<ModulateableParameter *> tTargets;

  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;

  void registerTarget(ModulateableParameter *target);
  void unregisterTarget(ModulateableParameter *target);

  void applyLpcPhysicalControl(tControlPositionValue diff);
  void applyAbsoluteLpcPhysicalControl(tControlPositionValue v);
  void onValueChanged(Initiator initiator, tControlPositionValue oldValue, tControlPositionValue newValue) override;

  void setUiSelectedHardwareSource(int paramID);
  void toggleUiSelectedHardwareSource(int inc = 1);
  int getUiSelectedHardwareSource() const;

  void undoableSetGivenName(const Glib::ustring &newName);
  void undoableSetGivenName(UNDO::Transaction *transaction, const ustring &newName);
  void undoableSetInfo(const Glib::ustring &newName);
  void undoableSetInfo(UNDO::Transaction *transaction, const Glib::ustring &newName);
  virtual void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount) override;
  void undoableResetConnectionsToTargets();
  const Glib::ustring &getGivenName() const;
  const Glib::ustring &getInfo() const;

  void loadDefault(UNDO::Transaction *transaction) override;
  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other) override;
  void copyTo(UNDO::Transaction *transaction, PresetParameter *other) const override;
  Glib::ustring getLongName() const override;

  const tTargets &getTargets() const;
  sigc::connection onTargetListChanged(sigc::slot<void> cb);

  bool isSourceOfTargetIn(const list<gint32> &ids) const;
  bool isSourceOf(gint32 id) const;

  virtual void onSelected() override;
  virtual void onUnselected() override;

  static int getLastSelectedMacroControl();

  virtual DFBLayout *createLayout(FocusAndMode focusAndMode) const override;

  virtual size_t getHash() const override;

 private:
  void updateBoundRibbon();

  tTargets m_targets;
  int m_UiSelectedHardwareSourceParameterID;
  Glib::ustring m_givenName;
  Glib::ustring m_info;

  sigc::signal<void> m_targetListChanged;
};
