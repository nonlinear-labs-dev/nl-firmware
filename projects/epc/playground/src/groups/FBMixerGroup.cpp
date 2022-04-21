#include "FBMixerGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear50DbScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar50DbScaleConverter.h"
#include "parameters/scale-converters/ParabolicGainDbScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar70DbScaleConverter.h"
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar1DbstScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/Linear70DbScaleConverter.h>

FBMixerGroup::FBMixerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "FB", vg }, "FB Mixer", "Feedback Mixer", "Feedback Mixer")
{
}

FBMixerGroup::~FBMixerGroup()
{
}

void FBMixerGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::FB_Mix_Comb, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::FB_Mix_SVF, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::FB_Mix_FX, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Rvb, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::FB_Mix_Drive, getVoiceGroup() }, ScaleConverter::get<Linear70DbScaleConverter>(),
      ScaleConverter::get<LinearBipolar70DbScaleConverter>(), (2.0 / 7.0), 140, 700));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Fold, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.5, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Asym, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0, 100, 1000));

  appendParameter(new Parameter(this, { C15::PID::FB_Mix_Lvl_KT, getVoiceGroup() }, ScaleConverter::get<LinearBipolar1DbstScaleConverter>(),
                                0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Lvl, getVoiceGroup() },
                                            ScaleConverter::get<ParabolicGainDbScaleConverter>(), 0.38, 100, 1000));

  appendParameter(new ModulateableParameter(
      this, { C15::PID::FB_Mix_Osc, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0.0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Osc_Src, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_Comb_Src, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_SVF_Src, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::FB_Mix_FX_Src, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));
}
