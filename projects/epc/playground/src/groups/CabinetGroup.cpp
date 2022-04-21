#include "CabinetGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/Linear50DbScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar50DbScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear20To100StScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar80StScaleConverter.h"
#include "parameters/scale-converters/Linear60To140StScaleConverter.h"
#include "parameters/scale-converters/LinearMinus50To0DbScaleConverter.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include "parameters/scale-converters/LinearBipolar50DbScaleConverter.h"
#include <parameters/ModulateableParameterWithUnusualModUnit.h>

CabinetGroup::CabinetGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Cab", vg }, "Cabinet", "Cabinet", "Cabinet")
{
}

CabinetGroup::~CabinetGroup()
{
}

void CabinetGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 188, getVoiceGroup() },
                                                              ScaleConverter::get<Linear50DbScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar50DbScaleConverter>()));

  appendParameter(new Parameter(this, { 190, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 191, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 192, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 194, getVoiceGroup() },
                                                              ScaleConverter::get<Linear60To140StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar80StScaleConverter>()));

  appendParameter(new Parameter(this, { 196, getVoiceGroup() }, ScaleConverter::get<Linear20To100StScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 197, getVoiceGroup() },
                                                              ScaleConverter::get<LinearMinus50To0DbScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar50DbScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 199, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));
}
