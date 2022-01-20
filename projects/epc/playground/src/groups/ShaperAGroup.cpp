#include "ShaperAGroup.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameters/scale-converters/Linear50DbScaleConverter.h"
#include "parameters/scale-converters/LinearBipolar50DbScaleConverter.h"
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearBipolar200PercentScaleConverter.h>
#include <parameters/ModulateableParameterWithUnusualModUnit.h>

ShaperAGroup::ShaperAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Sh A", vg }, "Shaper A", "Shaper A", "Shaper A")
{
}

ShaperAGroup::~ShaperAGroup()
{
}

void ShaperAGroup::init()
{
  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Shp_A_Drive, getVoiceGroup() }, ScaleConverter::get<Linear50DbScaleConverter>(),
      ScaleConverter::get<LinearBipolar50DbScaleConverter>(), 0.2, 100, 500));

  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Drive_Env_A, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Fold, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Asym, getVoiceGroup() },
                                ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Shp_A_Mix, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_FB_Mix, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_FB_Env_C, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(), 0,
                                100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Shp_A_Ring_Mod, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));
}
