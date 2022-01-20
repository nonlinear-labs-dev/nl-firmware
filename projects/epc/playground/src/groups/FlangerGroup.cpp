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
#include "parameter_declarations.h"

FlangerGroup::FlangerGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Flang", vg }, "Flanger", "Flanger", "Flanger")
{
}

FlangerGroup::~FlangerGroup()
{
}

void FlangerGroup::init()
{
  appendParameter(new ModulateableParameter(
      this, { C15::PID::Flanger_Time_Mod, getVoiceGroup() }, ScaleConverter::get<BipolarParabolic100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Phase, getVoiceGroup() }, ScaleConverter::get<Linear180DegreeScaleConverter>(),
                                0.5, 180, 1800));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Rate, getVoiceGroup() },
                                            ScaleConverter::get<Parabolic10HzScaleConverter>(), 0.317, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Time, getVoiceGroup() },
                                            ScaleConverter::get<Parabolic50MsScaleConverter>(), 0.317, 125, 1250));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Stereo, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar50PercentScaleConverter>(), 0, 50, 500));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Flanger_Feedback, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Cross_FB, getVoiceGroup() },
                                ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Hi_Cut, getVoiceGroup() }, ScaleConverter::get<Linear60To140StScaleConverter>(),
                                0.75, 80, 800));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Flanger_Mix, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Envelope, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(),
                                0.0, 100, 1000));

  appendParameter(new ModulateableParameter(
      this, { C15::PID::Flanger_AP_Mod, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0.0, 100, 1000));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Flanger_AP_Tune, getVoiceGroup() }, ScaleConverter::get<Linear0To140StScaleConverter>(),
      ScaleConverter::get<LinearBipolar140StScaleConverter>(), 1.0, 140, 1400));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Tremolo, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>(), 0.0, 100, 1000));
}
