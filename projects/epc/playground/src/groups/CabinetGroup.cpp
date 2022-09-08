#include "CabinetGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

CabinetGroup::CabinetGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Cab", vg }, "Cabinet", "Cabinet", "Cabinet")
{
}

CabinetGroup::~CabinetGroup() = default;

void CabinetGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Drive, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Fold, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Asym, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Tilt, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Hi_Cut, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Lo_Cut, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Cab_Lvl, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Cabinet_Mix, getVoiceGroup() }));
}
