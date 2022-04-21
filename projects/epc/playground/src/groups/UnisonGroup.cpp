#include "UnisonGroup.h"

#include "parameters/Parameter.h"

#include "parameters/scale-converters/Linear12CountScaleConverter.h"
#include "parameters/scale-converters/Linear360DegreeScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/Fine12STScaleConverter.h"
#include "parameters/scale-converters/FineBipolar12STScaleConverter.h"
#include "parameter_declarations.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/UnmodulatebaleParameterLayouts.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/scale-converters/dimension/VoicesDimension.h>
#include <parameters/UnisonVoicesParameter.h>
#include <parameters/unison-parameters/ModulateableUnisonParameterWithUnusualModUnit.h>

UnisonGroup::UnisonGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Unison", vg }, "Unison", "Unison", "Unison")
{
}

UnisonGroup::~UnisonGroup() = default;

void UnisonGroup::init()
{
  appendParameter(new UnisonVoicesParameter(this, getVoiceGroup()));

  appendParameter(new ModulateableUnisonParameterWithUnusualModUnit(
      this, ParameterId { C15::PID::Unison_Detune, getVoiceGroup() }, ScaleConverter::get<Fine12STScaleConverter>(),
      ScaleConverter::get<FineBipolar12STScaleConverter>()));

  appendParameter(new UnmodulateableUnisonParameter(this, ParameterId { C15::PID::Unison_Phase, getVoiceGroup() },
                                                    ScaleConverter::get<Linear360DegreeScaleConverter>()));

  appendParameter(new UnmodulateableUnisonParameter(this, ParameterId { C15::PID::Unison_Pan, getVoiceGroup() },
                                                    ScaleConverter::get<Linear100PercentScaleConverter>()));
}

bool UnisonGroup::isUnisonParameter(const Parameter *parameter)
{
  return isUnisonParameter(parameter->getID());
}

bool UnisonGroup::isUnisonParameter(const ParameterId &id)
{
  const auto n = id.getNumber();
  return n == C15::PID::Unison_Voices || n == C15::PID::Unison_Detune || n == C15::PID::Unison_Phase
      || n == C15::PID::Unison_Pan;
}

bool UnisonGroup::isUnisonVoicesParameter(const Parameter *parameter)
{
  return isUnisonVoicesParameter(parameter->getID());
}

bool UnisonGroup::isUnisonVoicesParameter(const ParameterId &id)
{
  return id.getNumber() == C15::PID::Unison_Voices;
}
