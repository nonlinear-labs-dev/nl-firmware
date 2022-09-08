#include "EchoGroup.h"

#include "../parameters/scale-converters/IdentityScaleConverter.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/ValueScaling.h"
#include "parameter_declarations.h"

EchoGroup::EchoGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Echo", vg }, "Echo", "Echo", "Echo")
{
}

EchoGroup::~EchoGroup()
{
}

void EchoGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Time, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Stereo, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Feedback, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Cross_FB, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Hi_Cut, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Mix, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Send, getVoiceGroup() }));
}
