#include "EchoGroup.h"

#include "../parameters/scale-converters/IdentityScaleConverter.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/ValueScaling.h"
#include "parameters/scale-converters/LinearBipolar33PercentScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear60To140StScaleConverter.h"
#include "parameter_declarations.h"
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
  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Time, getVoiceGroup() },
                                            ScaleConverter::get<Parabolic2000MsScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Echo_Stereo, getVoiceGroup() }, ScaleConverter::get<LinearBipolar33PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar66PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Feedback, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Cross_FB, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Hi_Cut, getVoiceGroup() },
                                            ScaleConverter::get<Linear60To140StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Mix, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Echo_Send, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));
}
