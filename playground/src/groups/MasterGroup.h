#pragma once

#include "ParameterGroup.h"

class MasterGroup : public ParameterGroup
{
 public:
  MasterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~MasterGroup();

  void init();

  virtual void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount) override;
};
