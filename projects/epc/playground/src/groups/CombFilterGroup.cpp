#include "CombFilterGroup.h"

#include "parameters/ModulateableParameter.h"
#include "parameters/Parameter.h"

#include "parameters/scale-converters/Fine105PercentScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/PitchCombLinearStScaleConverter.h"
#include "parameters/scale-converters/Linear0To140StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar60StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar120StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar140StScaleConverter.h"
#include "parameters/scale-converters/Linear40To140StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameters/scale-converters/FineBipolar80StScaleConverter.h"
#include "parameters/scale-converters/CombDecayBipolarMsScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100StScaleConverter.h"
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/scale-converters/PitchCombLinearStModulationScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>

CombFilterGroup::CombFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Comb", vg }, "Comb Filter", "Comb Filter", "Comb Filter")
{
}

CombFilterGroup::~CombFilterGroup()
{
}

void CombFilterGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_In_A_B, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Comb_Flt_Pitch, getVoiceGroup() }, ScaleConverter::get<PitchCombLinearStScaleConverter>(),
      ScaleConverter::get<PitchCombLinearStModulationScaleConverter>(), 0.5, 120, 12000));

  appendParameter(new Parameter(this, { C15::PID::Comb_Flt_Pitch_KT, getVoiceGroup() },
                                ScaleConverter::get<Fine200PercentScaleConverter>(), 0.5, 200, 20000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_Pitch_Env_C, getVoiceGroup() },
                                ScaleConverter::get<FineBipolar80StScaleConverter>(), ScaleConverter::get<FineBipolar80StScaleConverter>(), 0, 80, 8000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Decay, getVoiceGroup() },
                                            ScaleConverter::get<CombDecayBipolarMsScaleConverter>(), 0, 100, 1000));

  appendParameter(new Parameter(this, { C15::PID::Comb_Flt_Decay_KT, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0.33, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Decay_Gate, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Comb_Flt_AP_Tune, getVoiceGroup() }, ScaleConverter::get<Linear0To140StScaleConverter>(),
      ScaleConverter::get<LinearBipolar140StScaleConverter>(), 1.0, 140, 1400));

  appendParameter(new Parameter(this, { C15::PID::Comb_Flt_AP_KT, getVoiceGroup() },
                                ScaleConverter::get<Linear200PercentScaleConverter>(), 0.5, 200, 2000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_AP_Env_C, getVoiceGroup() },
                                ScaleConverter::get<FineBipolar80StScaleConverter>(), ScaleConverter::get<FineBipolar80StScaleConverter>(), 0, 80, 800));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_AP_Res, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Comb_Flt_LP_Tune, getVoiceGroup() }, ScaleConverter::get<Linear40To140StScaleConverter>(),
      ScaleConverter::get<LinearBipolar100StScaleConverter>(), 1.0, 100, 1000));

  appendParameter(new Parameter(this, { C15::PID::Comb_Flt_LP_KT, getVoiceGroup() },
                                ScaleConverter::get<Linear200PercentScaleConverter>(), 0.5, 200, 2000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_LP_Env_C, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100StScaleConverter>(), ScaleConverter::get<LinearBipolar100StScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Comb_Flt_PM, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_PM_A_B, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));
}
