#include <parameters/ModulateableParameter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <parameters/scale-converters/LinearBipolar96StScaleConverterFine.h>
#include "MasterGroup.h"
#include "parameters/Parameter.h"

#include "parameters/scale-converters/LinearBipolar48StScaleConverter.h"
#include "parameters/scale-converters/ParabolicGainDbScaleConverter.h"
#include "parameter_declarations.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"

#include <parameter_list.h>

MasterGroup::MasterGroup(ParameterGroupSet *parent)
    : ParameterGroup(parent, { "Master", VoiceGroup::Global }, "Master", "Master", "Master")
{
}

MasterGroup::~MasterGroup()
{
}

void MasterGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Master_Volume, VoiceGroup::Global },
                                            ScaleConverter::get<ParabolicGainDbScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Master_Tune, VoiceGroup::Global }, ScaleConverter::get<LinearBipolar48StScaleConverter>(),
      ScaleConverter::get<LinearBipolar96StScaleConverterFine>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Master_Pan, VoiceGroup::Global },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Master_Serial_FX, VoiceGroup::Global },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));
}

void MasterGroup::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount)
{
}

bool MasterGroup::isMasterParameter(const Parameter *p)
{
  if(p)
  {
    auto id = p->getID().getNumber();
    return id == C15::PID::Master_Volume || id == C15::PID::Master_Tune || id == C15::PID::Master_Serial_FX || id == C15::PID::Master_Pan;
  }
  return false;
}
