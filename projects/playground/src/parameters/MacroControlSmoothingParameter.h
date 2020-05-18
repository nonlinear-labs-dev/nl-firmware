#pragma once
#include "Parameter.h"

class MacroControlSmoothingParameter : public Parameter
{
 public:
  using Parameter::Parameter;
  Layout *createLayout(FocusAndMode focusAndMode) const override;

 protected:
  void setCpValue(UNDO::Transaction* transaction, Initiator initiator, tControlPositionValue value,
                  bool doSendToLpc) override;
 public:
  ParameterId getMC() const;
};