#include "SVFilterGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar120StScaleConverter.h"
#include "parameters/scale-converters/Linear20To140StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar60StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>

SVFilterGroup::SVFilterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup)
    : ParameterGroup(parent, "SVF", "SV Filter", "State Variable Filter", "State Variable Filter", voicegroup)
{
}

SVFilterGroup::~SVFilterGroup()
{
}

void SVFilterGroup::init()
{
  appendParameter(new ModulateableParameter(this, { 136, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 138, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 140, getVoiceGroup() }, ScaleConverter::get<Linear20To140StScaleConverter>(),
      ScaleConverter::get<LinearBipolar120StScaleConverter>(), 0.5, 120, 1200));

  appendParameter(new Parameter(this, { 142, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                1.0, 100, 1000));

  appendParameter(new Parameter(this, { 143, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100StScaleConverter>(),
                                0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { 144, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new Parameter(this, { 146, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new Parameter(this, { 147, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 148, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60StScaleConverter>(),
      ScaleConverter::get<LinearBipolar120StScaleConverter>(), 0.2, 60, 600));

  appendParameter(new ModulateableParameter(this, { 150, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new Parameter(this, { 152, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 153, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new Parameter(this, { 155, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.0, 100, 1000));
}
