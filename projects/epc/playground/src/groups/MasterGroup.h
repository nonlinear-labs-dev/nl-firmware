#pragma once

#include "ParameterGroup.h"

class MasterGroup : public ParameterGroup
{
 public:
  explicit MasterGroup(ParameterGroupSet *parent);

  void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount) override;

  static bool isMasterParameter(const Parameter *p);
};
