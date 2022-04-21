#include "ReverbGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameter_declarations.h"

ReverbGroup::ReverbGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Reverb", vg }, "Reverb", "Reverb", "Reverb")
{
}

ReverbGroup::~ReverbGroup()
{
}

void ReverbGroup::init()
{
  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Size, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.33, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Pre_Dly, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.33, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Color, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Chorus, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.25, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Mix, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Reverb_Send, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 1.0, 100, 1000));
}
