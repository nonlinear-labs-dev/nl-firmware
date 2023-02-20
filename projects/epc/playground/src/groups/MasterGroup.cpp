#include <parameters/ModulateableParameter.h>
#include "MasterGroup.h"
#include "parameter_declarations.h"
#include "parameter_group.h"
#include <parameter_list.h>

namespace
{
  auto getDesc()
  {
    return C15::ParameterGroups[static_cast<int>(C15::Descriptors::ParameterGroup::Master)];
  }
}

MasterGroup::MasterGroup(ParameterGroupSet *parent)
    : ParameterGroup(parent, getDesc(), VoiceGroup::Global)
{
}

void MasterGroup::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount)
{
}
