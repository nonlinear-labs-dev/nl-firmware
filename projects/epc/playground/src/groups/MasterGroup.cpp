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
