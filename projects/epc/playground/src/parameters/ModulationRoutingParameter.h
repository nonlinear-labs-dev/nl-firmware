#pragma once

#include "Parameter.h"

class MacroControlParameter;
class PhysicalControlParameter;

class ModulationRoutingParameter : public Parameter, public IntrusiveListItem<ModulationRoutingParameter *>
{
  typedef Parameter super;

  typedef PhysicalControlParameter *tSrcParameterPtr;
  typedef MacroControlParameter *tMCParameterPtr;

  tMCParameterPtr m_tgtParameter;
  tSrcParameterPtr m_srcParameter;

 public:
  ModulationRoutingParameter(ParameterGroup *group, const ParameterId& id, tSrcParameterPtr srcParam, tMCParameterPtr tgtParam);
  ~ModulationRoutingParameter() override;

  void applyPlaycontrollerPhysicalControl(tControlPositionValue diff);
  void applyAbsolutePlaycontrollerPhysicalControl(tControlPositionValue v);

  void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount) override;

  tMCParameterPtr getTargetParameter() const;
  tSrcParameterPtr getSourceParameter() const;

  bool routes(const PhysicalControlParameter *p) const;
  Glib::ustring getDisplayString() const override;
  tControlPositionValue getControlPositionValue() const override;
  Layout *createLayout(FocusAndMode focusAndMode) const override;

  void onExclusiveRoutingLost();
  void onExclusiveRoutingLost(UNDO::Transaction* transaction);

 protected:
  void setCpValue(UNDO::Transaction *transaction, Initiator initiator, tControlPositionValue value,
                  bool dosendToPlaycontroller) override;
};
