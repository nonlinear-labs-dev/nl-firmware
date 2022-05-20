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
  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Time_Mod, getVoiceGroup() },
                                            ScaleConverter::get<BipolarParabolic100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Phase, getVoiceGroup() },
                                            ScaleConverter::get<Linear180DegreeScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Rate, getVoiceGroup() },
                                            ScaleConverter::get<Parabolic10HzScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Time, getVoiceGroup() },
                                            ScaleConverter::get<Parabolic50MsScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Stereo, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar50PercentScaleConverter>()));

  appendParameter(
      new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Flanger_Feedback, getVoiceGroup() },
                                                  ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
                                                  ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Cross_FB, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Hi_Cut, getVoiceGroup() },
                                            ScaleConverter::get<Linear60To140StScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(
      this, { C15::PID::Flanger_Mix, getVoiceGroup() }, ScaleConverter::get<LinearBipolar100PercentScaleConverter>(),
      ScaleConverter::get<LinearBipolar200PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Envelope, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_AP_Mod, getVoiceGroup() },
                                            ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulateableParameterWithUnusualModUnit(this, { C15::PID::Flanger_AP_Tune, getVoiceGroup() },
                                                              ScaleConverter::get<Linear0To140StScaleConverter>(),
                                                              ScaleConverter::get<LinearBipolar140StScaleConverter>()));

  appendParameter(new ModulateableParameter(this, { C15::PID::Flanger_Tremolo, getVoiceGroup() },
                                            ScaleConverter::get<Linear100PercentScaleConverter>()));
}
