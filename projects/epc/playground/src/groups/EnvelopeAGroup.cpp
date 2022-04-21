#include <groups/EnvelopeAGroup.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h>
#include <parameters/scale-converters/EnvelopeReleaseTimeMSScaleConverter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/Linear60DbScaleConverter.h>
#include <parameters/scale-converters/Linear60DbtScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar1DbstScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar24DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar48DbScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar60DbScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModDenominator.h>
#include <parameters/scale-converters/LinearBipolar60DBTScaleConverter.h>

EnvelopeAGroup::EnvelopeAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env A", vg }, "Envelope A", "Envelope A", "Envelope A")
{
}

EnvelopeAGroup::~EnvelopeAGroup()
{
}

void EnvelopeAGroup::init()
{
  appendParameter(new ModulateableParameter(this, { 0, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(
      new Parameter(this, { 294, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { 2, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 4, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { 6, getVoiceGroup() },
                                            ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>()));

  appendParameter(
      new ModulateableParameter(this, { 8, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModDenominator(
      this, { 10, getVoiceGroup() }, ScaleConverter::get<EnvelopeReleaseTimeMSScaleConverter>(), 0.53, 101, 1010, 100,
      1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { 12, getVoiceGroup() },
                                                              ScaleConverter::get<LinearBipolar24DbScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar48DbScaleConverter>()));

  appendParameter(new Parameter(this, { 14, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbScaleConverter>()));

  appendParameter(new Parameter(this, { 15, getVoiceGroup() }, ScaleConverter::get<Linear60DbtScaleConverter>()));

  appendParameter(new Parameter(this, { 16, getVoiceGroup() }, ScaleConverter::get<Linear60DbtScaleConverter>()));

  appendParameter(
      new Parameter(this, { 17, getVoiceGroup() }, ScaleConverter::get<LinearBipolar1DbstScaleConverter>()));

  appendParameter(new Parameter(this, { 18, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(
      new Parameter(this, { 328, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(
      new Parameter(this, { 330, getVoiceGroup() }, ScaleConverter::get<LinearBipolar60DbtScaleConverter>()));

  appendParameter(
      new Parameter(this, { 332, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));
}
