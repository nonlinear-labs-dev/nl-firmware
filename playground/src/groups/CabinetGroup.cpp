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

CabinetGroup::CabinetGroup(ParameterDualGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Cab", vg }, "Cabinet", "Cabinet", "Cabinet")
{
}

CabinetGroup::~CabinetGroup()
{
}

void CabinetGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 188, getVoiceGroup() }, ScaleConverter::get<Linear50DbScaleConverter>(),
      ScaleConverter::get<LinearBipolar50DbScaleConverter>(), 0.4, 50, 500));

  appendParameter(new Parameter(this, { 190, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.25, 100, 1000));

  appendParameter(new Parameter(this, { 191, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.25, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 192, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), -0.2, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 194, getVoiceGroup() }, ScaleConverter::get<Linear60To140StScaleConverter>(),
      ScaleConverter::get<LinearBipolar80StScaleConverter>(), 50.0 / 80.0, 80, 800));

  appendParameter(new Parameter(this, { 196, getVoiceGroup() }, ScaleConverter::get<Linear20To100StScaleConverter>(),
                                10.0 / 80.0, 80, 800));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 197, getVoiceGroup() }, ScaleConverter::get<LinearMinus50To0DbScaleConverter>(),
      ScaleConverter::get<LinearBipolar50DbScaleConverter>(), 36.0 / 50.0, 50, 500));

  appendParameter(new ModulateableParameter(this, { 199, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));
}
