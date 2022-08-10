#include "OscillatorBGroup.h"

#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/IdentityScaleConverter.h"
#include "parameters/scale-converters/PitchOscLinearStScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar150StScaleConverter.h"
#include "parameters/scale-converters/PhaseBipolar180DegreeScaleConverter.h"
#include "parameters/scale-converters/PhaseBipolar360DegreeScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/Fine105PercentScaleConverter.h"
#include "parameters/scale-converters/Linear80To140StScaleConverter.h"
#include "parameters/scale-converters/IdentityScaleConverter.h"
#include "parameters/scale-converters/FineBipolar80StScaleConverter.h"
#include "parameter_declarations.h"
#include "parameters/scale-converters/FineBipolar160StScaleConverter.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/scale-converters/PitchOscLinearStScaleModulationConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/OnOffScaleConverter.h>

OscillatorBGroup::OscillatorBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Osc B", vg }, "Oscillator B", "Oscillator B", "Oscillator B")
{
}

OscillatorBGroup::~OscillatorBGroup()
{
}

void OscillatorBGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_B_Pitch, getVoiceGroup() }, ScaleConverter::get<PitchOscLinearStScaleConverter>(),
      ScaleConverter::get<PitchOscLinearStScaleModulationConverter>()));

  appendParameter(new Parameter(this, { C15::PID::Osc_B_Pitch_KT, getVoiceGroup() },
                                ScaleConverter::get<Fine200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Osc_B_Pitch_Env_C, getVoiceGroup() },
                                                              ScaleConverter::get<FineBipolar80StScaleConverter>(),
                                                              ScaleConverter::get<FineBipolar160StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Fluct, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Fluct_Env_C, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_B_PM_Self, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_Self_Env_B, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Osc_B_PM_Self_Shp, getVoiceGroup() },
                                                  ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
                                                  ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_B_PM_A, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_A_Env_A, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_B_PM_A_Shp, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_B_PM_FB, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_PM_FB_Env_C, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_B_Phase, getVoiceGroup() }, ScaleConverter::get<PhaseBipolar180DegreeScaleConverter>(),
      ScaleConverter::get<PhaseBipolar360DegreeScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_B_Chirp, getVoiceGroup() },
                                            ScaleConverter::get<Linear80To140StScaleConverter>()));

  appendParameter(
      new Parameter(this, { C15::PID::Osc_B_Reset, getVoiceGroup() }, ScaleConverter::get<OnOffScaleConverter>()));
}
