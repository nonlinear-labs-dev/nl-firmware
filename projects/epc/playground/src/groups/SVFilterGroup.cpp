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

SVFilterGroup::SVFilterGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "SVF", vg }, "SV Filter", "State Variable Filter", "State Variable Filter")
{
}

SVFilterGroup::~SVFilterGroup()
{
}

void SVFilterGroup::init()
{
  appendParameter(
      new ModulateableParameter(this, { 136, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 138, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 140, getVoiceGroup() },
                                                              ScaleConverter::get<Linear20To140StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar120StScaleConverter>()));

  appendParameter(new Parameter(this, { 142, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 143, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100StScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 144, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 146, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 147, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 148, getVoiceGroup() },
                                                              ScaleConverter::get<LinearBipolar60StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar120StScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 150, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 152, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 153, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 155, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));
}
