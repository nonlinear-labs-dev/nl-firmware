#include <groups/EnvelopeAGroup.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeReleaseTimeMSScaleConverter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/Linear60DbScaleConverter.h>
#include <parameters/scale-converters/Linear60DbtScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar1DbstScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar24DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar60DbScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar60DBTScaleConverter.h>
#include "parameter_declarations.h"

EnvelopeAGroup::EnvelopeAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env A", vg }, "Envelope A", "Envelope A", "Envelope A")
{
}

EnvelopeAGroup::~EnvelopeAGroup()
{
}

void EnvelopeAGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Att, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Dec_1, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_BP, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Dec_2, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Sus, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Rel, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeReleaseTimeMSScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Env_A_Gain, getVoiceGroup() },
                                                              ScaleConverter::get<LinearBipolar24DbScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar48DbScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Lvl_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Att_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Dec_1_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Dec_2_Vel, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Rel_Vel, getVoiceGroup() },
                                ScaleConverter::get<Linear60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Lvl_KT, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar1DbstScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Time_KT, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Att_Curve, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Elevate, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));
}
