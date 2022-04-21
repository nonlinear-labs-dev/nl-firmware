#include "OscillatorAGroup.h"

#include "../parameters/scale-converters/IdentityScaleConverter.h"

#include "../parameters/scale-converters/FineBipolar80StScaleConverter.h"

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
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/scale-converters/PitchOscLinearStScaleModulationConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/OnOffScaleConverter.h>

OscillatorAGroup::OscillatorAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Osc A", vg }, "Oscillator A", "Oscillator A", "Oscillator A")
{
}

OscillatorAGroup::~OscillatorAGroup()
{
}

void OscillatorAGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_A_Pitch, getVoiceGroup() }, ScaleConverter::get<PitchOscLinearStScaleConverter>(),
      ScaleConverter::get<PitchOscLinearStScaleModulationConverter>(), 80.0 / 150.0, 150, 15000));

  appendParameter(new Parameter(this, { C15::PID::Osc_A_Pitch_KT, getVoiceGroup() },
                                ScaleConverter::get<Fine200PercentScaleConverter>(), 0.5, 200, 20000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Osc_A_Pitch_Env_C, getVoiceGroup() },
                                ScaleConverter::get<FineBipolar80StScaleConverter>(), ScaleConverter::get<FineBipolar80StScaleConverter>(), 0, 80, 8000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Fluct, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Fluct_Env_C, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_A_PM_Self, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_Self_Env_A, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_Self_Shp, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_A_PM_B, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_B_Env_B, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_B_Shp, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_A_PM_FB, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_PM_FB_Env_C, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Osc_A_Phase, getVoiceGroup() }, ScaleConverter::get<PhaseBipolar180DegreeScaleConverter>(),
      ScaleConverter::get<PhaseBipolar360DegreeScaleConverter>(), 0, 180, 1800));

  appendParameter(new ModulateableParameter(this, { C15::PID::Osc_A_Chirp, getVoiceGroup() }, ScaleConverter::get<Linear80To140StScaleConverter>(),
                                4.0 / 6.0, 60, 600));

  appendParameter(new Parameter(this, { C15::PID::Osc_A_Reset, getVoiceGroup() }, ScaleConverter::get<OnOffScaleConverter>(), 1, 1, 1));
}
