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
#include <parameters/ModulateableParameterWithUnusualModDenominator.h>
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
  appendParameter(new ModulateableParameter(
      this, { C15::PID::Env_A_Att, getVoiceGroup() }, ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Att_Curve, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(
      this, { C15::PID::Env_A_Dec_1, getVoiceGroup() }, ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.59, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_BP, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new ModulateableParameter(
      this, { C15::PID::Env_A_Dec_2, getVoiceGroup() }, ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.79, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Sus, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModDenominator(
      this, { C15::PID::Env_A_Rel, getVoiceGroup() }, ScaleConverter::get<EnvelopeReleaseTimeMSScaleConverter>(), 0.53, 101, 1010, 100,
      1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Env_A_Gain, getVoiceGroup() }, ScaleConverter::get<LinearBipolar24DbScaleConverter>(),
      ScaleConverter::get<LinearBipolar48DbScaleConverter>(), 0, 48, 480));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Lvl_Vel, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbScaleConverter>(),
                                0.5, 60, 600));

  appendParameter(
      new Parameter(this, { C15::PID::Env_A_Att_Vel, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbtScaleConverter>(), 0.5, 60, 600));

  appendParameter(
      new Parameter(this, { C15::PID::Env_A_Rel_Vel, getVoiceGroup() }, ScaleConverter::get<Linear60DbtScaleConverter>(), 0, 60, 600));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Lvl_KT, getVoiceGroup() }, ScaleConverter::get<LinearBipolar1DbstScaleConverter>(),
                                0, 100, 1000));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Time_KT, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
                                0.05, 100, 1000));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Dec_1_Vel, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbtScaleConverter>(),
                                0.0, 60, 600));

  appendParameter(new Parameter(this, { C15::PID::Env_A_Dec_2_Vel, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbtScaleConverter>(),
                                0.0, 60, 600));

  appendParameter(new ModulateableParameter(this, { C15::PID::Env_A_Elevate, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0.0, 100, 1000));
}
