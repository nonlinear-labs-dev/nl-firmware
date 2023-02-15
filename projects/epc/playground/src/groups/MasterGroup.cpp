#include <parameters/ModulateableParameter.h>
#include "MasterGroup.h"
#include "parameter_declarations.h"
#include <parameter_list.h>

MasterGroup::MasterGroup(ParameterGroupSet *parent)
    : ParameterGroup(parent, { "Master", VoiceGroup::Global }, "Master", "Master", "Master")
{
}

void MasterGroup::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount)
{
}

bool MasterGroup::isMasterParameter(const Parameter *p)
{
  if(p)
  {
    auto id = p->getID().getNumber();
    return id == C15::PID::Master_Volume || id == C15::PID::Master_Tune || id == C15::PID::Master_Serial_FX
        || id == C15::PID::Master_Pan || id == C15::PID::Master_FX_Mix;
  }
  return false;
}
