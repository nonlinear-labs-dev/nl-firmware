#include "OutputMixerGroup.h"

#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar50DbScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/ParabolicGainDbScaleConverter.h"
#include "parameters/scale-converters/Linear50DbScaleConverter.h"
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>

OutputMixerGroup::OutputMixerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Mixer", vg }, "Output Mixer", "Output Mixer", "Output Mixer")
{
}

OutputMixerGroup::~OutputMixerGroup()
{
}

void OutputMixerGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Out_Mix_A_Lvl, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_A_Pan, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Out_Mix_B_Lvl, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_B_Pan, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Out_Mix_Comb_Lvl, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Comb_Pan, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Out_Mix_SVF_Lvl, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_SVF_Pan, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Out_Mix_Drive, getVoiceGroup() }, ScaleConverter::get<Linear50DbScaleConverter>(),
      ScaleConverter::get<LinearBipolar50DbScaleConverter>(), 0.0, 100, 500));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Fold, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.5, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Asym, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_Lvl, getVoiceGroup() },
                                            ScaleConverter::get<ParabolicGainDbScaleConverter>(), 0.38, 100, 1000));

  appendParameter(new Parameter(this, { C15::PID::Out_Mix_Key_Pan, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Out_Mix_To_FX, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));
}
