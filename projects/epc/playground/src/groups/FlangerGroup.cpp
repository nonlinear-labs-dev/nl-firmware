#include <groups/FlangerGroup.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/BipolarParabolic100PercentScaleConverter.h>
#include <parameters/scale-converters/Linear0To140StScaleConverter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/Linear180DegreeScaleConverter.h>
#include <parameters/scale-converters/Linear60To140StScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar50PercentScaleConverter.h>
#include <parameters/scale-converters/Parabolic10HzScaleConverter.h>
#include <parameters/scale-converters/Parabolic50MsScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar140StScaleConverter.h>

FlangerGroup::FlangerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Flang", vg }, "Flanger", "Flanger", "Flanger")
{
}

FlangerGroup::~FlangerGroup()
{
}

void FlangerGroup::init()
{
  appendParameter(new ModulateableParameter(this, { 211, getVoiceGroup() },
                                            ScaleConverter::get<BipolarParabolic100PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 213, getVoiceGroup() }, ScaleConverter::get<Linear180DegreeScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 214, getVoiceGroup() }, ScaleConverter::get<Parabolic10HzScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 216, getVoiceGroup() }, ScaleConverter::get<Parabolic50MsScaleConverter>()));

  appendParameter(
      new Parameter(this, { 218, getVoiceGroup() }, ScaleConverter::get<LinearBipolar50PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 219, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 221, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 222, getVoiceGroup() }, ScaleConverter::get<Linear60To140StScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { 223, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new Parameter(this, { 307, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { 308, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 310, getVoiceGroup() },
                                                              ScaleConverter::get<Linear0To140StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar140StScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 389, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));
}
