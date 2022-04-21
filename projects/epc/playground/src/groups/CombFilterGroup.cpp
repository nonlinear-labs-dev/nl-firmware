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
  appendParameter(
      new ModulateableParameter(this, { 113, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 115, getVoiceGroup() }, ScaleConverter::get<PitchCombLinearStScaleConverter>(),
      ScaleConverter::get<PitchCombLinearStModulationScaleConverter>()));

  appendParameter(new Parameter(this, { 117, getVoiceGroup() }, ScaleConverter::get<Fine105PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 118, getVoiceGroup() }, ScaleConverter::get<FineBipolar80StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { 119, getVoiceGroup() },
                                            ScaleConverter::get<CombDecayBipolarMsScaleConverter>()));

  appendParameter(new Parameter(this, { 121, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 305, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 123, getVoiceGroup() },
                                                              ScaleConverter::get<Linear0To140StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar140StScaleConverter>()));

  appendParameter(new Parameter(this, { 125, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 126, getVoiceGroup() }, ScaleConverter::get<FineBipolar80StScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 127, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 129, getVoiceGroup() },
                                                              ScaleConverter::get<Linear40To140StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar100StScaleConverter>()));

  appendParameter(new Parameter(this, { 131, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 132, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100StScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 133, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 135, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));
}
