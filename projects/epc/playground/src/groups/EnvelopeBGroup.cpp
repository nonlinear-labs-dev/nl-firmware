#include <groups/EnvelopeBGroup.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeReleaseTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeTimeMSScaleConverter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/Linear60DbScaleConverter.h>
#include <parameters/scale-converters/Linear60DbtScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar1DbstScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar24DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48DbScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModDenominator.h>
#include <parameters/scale-converters/LinearBipolar60DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar60DBTScaleConverter.h>

EnvelopeBGroup::EnvelopeBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env B", vg }, "Envelope B", "Envelope B", "Envelope B")
{
}

EnvelopeBGroup::~EnvelopeBGroup()
{
}

void EnvelopeBGroup::init()
{
  appendParameter(new ModulateableParameter(this, { 19, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(
      new Parameter(this, { 295, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { 21, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 23, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { 25, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 27, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModDenominator(
      this, { 29, getVoiceGroup() }, ScaleConverter::get<EnvelopeReleaseTimeMSScaleConverter>(), 0.53, 101, 1010, 100,
      1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 31, getVoiceGroup() },
                                                              ScaleConverter::get<LinearBipolar24DbScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar48DbScaleConverter>()));

  appendParameter(new Parameter(this, { 33, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbScaleConverter>()));

  appendParameter(new Parameter(this, { 34, getVoiceGroup() }, ScaleConverter::get<Linear60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { 35, getVoiceGroup() }, ScaleConverter::get<Linear60DbtScaleConverter>()));

  appendParameter(
      new Parameter(this, { 36, getVoiceGroup() }, ScaleConverter::get<LinearBipolar1DbstScaleConverter>()));

  appendParameter(new Parameter(this, { 37, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 334, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(
      new Parameter(this, { 336, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(
      new Parameter(this, { 338, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));
}
