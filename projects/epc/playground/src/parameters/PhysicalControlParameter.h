#pragma once

#include "Parameter.h"
#include "HardwareSourceSendParameter.h"

class ModulationRoutingParameter;

class PhysicalControlParameter : public Parameter
{
  typedef Parameter super;

 public:
  PhysicalControlParameter(ParameterGroup *group, ParameterId id);

  bool isChangedFromLoaded() const override;
  virtual void onChangeFromExternalSource(tControlPositionValue newValue, HWChangeSource source);

  void registerTarget(ModulationRoutingParameter *target);
  Glib::ustring generateName() const;
  void loadFromPreset(UNDO::Transaction *transaction, const tControlPositionValue &value) override;

  void setCPFromHwui(UNDO::Transaction *transaction, const tControlPositionValue &cpValue) override;
  void setCPFromWebUI(UNDO::Transaction *transaction, const tControlPositionValue &cpValue) override;

  Glib::ustring getDisplayString() const override;
  Glib::ustring getCurrentModulatingMacroControlString() const;
  bool isMCAssignedToThisAlsoAssignedToAnyPedal() const;

  virtual ReturnMode getReturnMode() const = 0;

  Layout *createLayout(FocusAndMode focusAndMode) const override;
  bool lockingEnabled() const override;
  void setUiSelectedModulationRouter(const ParameterId &paramNumber);
  void toggleUiSelectedModulationRouter(int inc);
  ParameterId getUiSelectedModulationRouter() const;

  void onSelected() override;
  void onUnselected() override;

  virtual bool hasBehavior() const;
  virtual Glib::ustring getCurrentBehavior() const;
  virtual void undoableStepBehavior(UNDO::Transaction *transaction, int direction);

  void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount) override;

  size_t getHash() const override;
  bool isLocked() const override;
  virtual bool isLocalEnabled() const = 0;
  HardwareSourceSendParameter *getSendParameter() const;

  virtual tControlPositionValue getDefValueAccordingToMode() const = 0;
  bool isValueChangedFromLoaded() const override;
  [[nodiscard]] double getLastControlPositionValueBeforePresetLoad() const;
  [[nodiscard]] ReturnMode getLastReturnModeBeforePresetLoad() const;

  virtual void onLocalEnableChanged(bool b) = 0;

  MacroControlParameter* getMCAssignedToThisHW() const;

 protected:
  void onValueChanged(Initiator initiator, tControlPositionValue oldValue, tControlPositionValue newValue) override;

 private:
  IntrusiveList<ModulationRoutingParameter *> m_targets;

  tControlPositionValue m_valueBeforeLastLoad = 0;
  ReturnMode m_returnModeBeforeLastLoad = ReturnMode::None;

  bool m_changingFromHWUI = false;
  bool m_lastChangedFromHWUI = false;
  ReturnMode m_oldReturnMode;
};
