#include "EchoGroup.h"

#include "../parameters/scale-converters/IdentityScaleConverter.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/ValueScaling.h"
#include "parameters/scale-converters/LinearBipolar33PercentScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear60To140StScaleConverter.h"
#include <parameters/scale-converters/LinearBipolar66PercentScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/Parabolic2000MsScaleConverter.h>

EchoGroup::EchoGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Echo", vg }, "Echo", "Echo", "Echo")
{
}

EchoGroup::~EchoGroup()
{
}

void EchoGroup::init()
{
  appendParameter(
      new ModulateableParameter(this, { 225, getVoiceGroup() }, ScaleConverter::get<Parabolic2000MsScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 227, getVoiceGroup() }, ScaleConverter::get<LinearBipolar33PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar66PercentScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 229, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 231, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 232, getVoiceGroup() }, ScaleConverter::get<Linear60To140StScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 233, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 342, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));
}
