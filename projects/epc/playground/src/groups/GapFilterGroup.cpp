#include "GapFilterGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/Linear24To120StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar96StScaleConverterCoarse.h"
#include "parameters/scale-converters/LinearBipolar36StScaleConverter.h"
#include "parameters/scale-converters/Linear96StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>

GapFilterGroup::GapFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Gap Filt", vg }, "Gap Filter", "Gap Filter", "Gap Filter")
{
}

GapFilterGroup::~GapFilterGroup()
{
}

void GapFilterGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Gap_Flt_Center, getVoiceGroup() }, ScaleConverter::get<Linear24To120StScaleConverter>(),
      ScaleConverter::get<LinearBipolar96StScaleConverterCoarse>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Stereo, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar36StScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Gap_Flt_Gap, getVoiceGroup() }, ScaleConverter::get<Linear96StScaleConverter>(),
      ScaleConverter::get<LinearBipolar96StScaleConverterCoarse>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Gap_Flt_Res, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Gap_Flt_Bal, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Gap_Flt_Mix, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));
}
