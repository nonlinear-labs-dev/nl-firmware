#include <groups/EnvelopeCGroup.h>
#include <parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeReleaseTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeTimeMSScaleConverter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/Linear60DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar1DbstScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar60DbScaleConverter.h>
#include <parameter_declarations.h>
#include "parameters/scale-converters/LinearBipolarInverted60DBTScaleConverter.h"

EnvelopeCGroup::EnvelopeCGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env C", vg }, "Envelope C", "Envelope C", "Envelope C")
{
}

EnvelopeCGroup::~EnvelopeCGroup()
{
}

void EnvelopeCGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Att, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Dec_1, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_BP, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Dec_2, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Sus, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Rel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Lvl_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Att_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Rel_Vel, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Lvl_KT, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::Env_C_Time_KT, getVoiceGroup() }));
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_C_Att_Curve, getVoiceGroup() }));
  appendParameter(new Parameter(this, { C15::PID::ParameterID::Env_C_Retr_H, getVoiceGroup() }));
}
