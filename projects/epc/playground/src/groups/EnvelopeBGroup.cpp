#include <groups/EnvelopeBGroup.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeReleaseTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeTimeMSScaleConverter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/Linear60DbScaleConverter.h>
#include <parameters/scale-converters/Linear60DbtScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar1DbstScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar24DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48DbScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar60DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar60DBTScaleConverter.h>
#include "parameter_declarations.h"

EnvelopeBGroup::EnvelopeBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env B", vg }, "Envelope B", "Envelope B", "Envelope B")
{
}

EnvelopeBGroup::~EnvelopeBGroup()
{
}

void EnvelopeBGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Att, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Dec_1, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_BP, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Dec_2, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Sus, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Rel, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeReleaseTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Env_B_Gain, getVoiceGroup() },
                                                              ScaleConverter::get<LinearBipolar24DbScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar48DbScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_B_Lvl_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_B_Att_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_B_Dec_1_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_B_Dec_2_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_B_Rel_Vel, getVoiceGroup() },
                                ScaleConverter::get<Linear60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_B_Lvl_KT, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar1DbstScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_B_Time_KT, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Att_Curve, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_B_Elevate, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));
}
