#pragma once

#include "PhysicalControlParameter.h"

class AftertouchParameter : public PhysicalControlParameter
{
 private:
  typedef PhysicalControlParameter super;

 public:
  using super::super;

  ReturnMode getReturnMode() const override;
  tControlPositionValue getDefValueAccordingToMode() const override;

 protected:
  Layout *createLayout(FocusAndMode focusAndMode) const override;

 private:
  bool isLocalEnabled() const override;
};
