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
  using SC = ScaleConverter;
  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_In_A_B, getVoiceGroup() },
                                            SC::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_Pitch, getVoiceGroup() },
                                                              SC::get<PitchCombLinearStScaleConverter>(),
                                                              SC::get<PitchCombLinearStModulationScaleConverter>()));

  appendParameter(
      new Parameter(this, { C15::PID::Comb_Flt_Pitch_KT, getVoiceGroup() }, SC::get<Fine200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_Pitch_Env_C, getVoiceGroup() },
                                                              SC::get<FineBipolar80StScaleConverter>(),
                                                              SC::get<FineBipolar160StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Decay, getVoiceGroup() },
                                            SC::get<CombDecayBipolarMsScaleConverter>()));

  appendParameter(
      new Parameter(this, { C15::PID::Comb_Flt_Decay_KT, getVoiceGroup() }, SC::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_Decay_Gate, getVoiceGroup() },
                                            SC::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_AP_Tune, getVoiceGroup() },
                                                              SC::get<Linear0To140StScaleConverter>(),
                                                              SC::get<LinearBipolar140StScaleConverter>()));

  appendParameter(
      new Parameter(this, { C15::PID::Comb_Flt_AP_KT, getVoiceGroup() }, SC::get<Linear200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_AP_Env_C, getVoiceGroup() },
                                                              SC::get<FineBipolar80StScaleConverter>(),
                                                              ScaleConverter::get<FineBipolar80StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_AP_Res, getVoiceGroup() },
                                            SC::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_LP_Tune, getVoiceGroup() },
                                                              SC::get<Linear40To140StScaleConverter>(),
                                                              SC::get<LinearBipolar100StScaleConverter>()));

  appendParameter(
      new Parameter(this, { C15::PID::Comb_Flt_LP_KT, getVoiceGroup() }, SC::get<Linear200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_LP_Env_C, getVoiceGroup() },
                                                              SC::get<LinearBipolar100StScaleConverter>(),
                                                              SC::get<LinearBipolar100StScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Comb_Flt_PM, getVoiceGroup() },
                                                              SC::get<LinearBipolar100PercentScaleConverter>(),
                                                              SC::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Comb_Flt_PM_A_B, getVoiceGroup() },
                                            SC::get<Linear100PercentScaleConverter>()));
}
