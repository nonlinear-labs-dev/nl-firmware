#pragma once

#include "PhysicalControlParameter.h"

class PitchbendParameter : public PhysicalControlParameter
{
 private:
  typedef PhysicalControlParameter super;

 public:
  using super::super;

  ReturnMode getReturnMode() const override;
  tControlPositionValue getDefValueAccordingToMode() const override;
  void onLocalEnableChanged(bool localEnableState) override;
  void loadFromPreset(UNDO::Transaction *transaction, const tControlPositionValue &value) override;

 private:
  bool isLocalEnabled() const override;
};
