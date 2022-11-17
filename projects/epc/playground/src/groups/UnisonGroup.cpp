#include "UnisonGroup.h"

#include "parameters/Parameter.h"
#include "parameter_declarations.h"
#include <proxies/hwui/HWUI.h>
#include <parameters/scale-converters/dimension/VoicesDimension.h>
#include <parameters/UnisonVoicesParameter.h>

UnisonGroup::UnisonGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Unison", vg }, "Unison", "Unison", "Unison")
{
}

UnisonGroup::~UnisonGroup() = default;

void UnisonGroup::init()
{
  appendParameter(new UnisonVoicesParameter(this, getVoiceGroup()));
  appendParameter(new ModulateableUnisonParameter(this, ParameterId { C15::PID::Unison_Detune, getVoiceGroup() }));
  appendParameter(new ModulateableUnisonParameter(this, ParameterId { C15::PID::Unison_Phase, getVoiceGroup() }));
  appendParameter(new ModulateableUnisonParameter(this, ParameterId { C15::PID::Unison_Pan, getVoiceGroup() }));
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
