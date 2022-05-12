#include "SVFilterGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar120StScaleConverter.h"
#include "parameters/scale-converters/Linear20To140StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar60StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>

SVFilterGroup::SVFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "SVF", vg }, "SV Filter", "State Variable Filter", "State Variable Filter")
{
}

SVFilterGroup::~SVFilterGroup()
{
}

void SVFilterGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_In_A_B, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new ModulateableParameterWithUnusualModUnit(this, { C15::PID::SV_Flt_Comb_Mix, getVoiceGroup() },
                                                  ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
                                                  ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::SV_Flt_Cut, getVoiceGroup() },
                                                              ScaleConverter::get<Linear20To140StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar120StScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::SV_Flt_Cut_KT, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Cut_Env_C, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Res, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new Parameter(this, { C15::PID::SV_Flt_Res_KT, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Res_Env_C, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::SV_Flt_Spread, getVoiceGroup() },
                                                              ScaleConverter::get<LinearBipolar60StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar120StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_LBH, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_Par, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::SV_Flt_FM, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::SV_Flt_FM_A_B, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));
}
