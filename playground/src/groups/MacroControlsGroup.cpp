#include "MacroControlsGroup.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h"

MacroControlsGroup::MacroControlsGroup(ParameterDualGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "MCs", vg }, "Macro Control", "Macro Control", "Macro Control")
{
}

MacroControlsGroup::~MacroControlsGroup()
{
}

void MacroControlsGroup::init()
{

  appendParameter(new MacroControlParameter(this, { 243, getVoiceGroup() }));
  appendParameter(new MacroControlParameter(this, { 244, getVoiceGroup() }));
  appendParameter(new MacroControlParameter(this, { 245, getVoiceGroup() }));
  appendParameter(new MacroControlParameter(this, { 246, getVoiceGroup() }));
  appendParameter(new MacroControlParameter(this, { 369, getVoiceGroup() }));
  appendParameter(new MacroControlParameter(this, { 371, getVoiceGroup() }));

  appendParameter(new Parameter(this, { 324, getVoiceGroup() },
                                ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.4, 100, 1000));
  appendParameter(new Parameter(this, { 325, getVoiceGroup() },
                                ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.4, 100, 1000));
  appendParameter(new Parameter(this, { 326, getVoiceGroup() },
                                ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.4, 100, 1000));
  appendParameter(new Parameter(this, { 327, getVoiceGroup() },
                                ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.4, 100, 1000));
  appendParameter(new Parameter(this, { 370, getVoiceGroup() },
                                ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.4, 100, 1000));
  appendParameter(new Parameter(this, { 372, getVoiceGroup() },
                                ScaleConverter::get<EnvelopeAttackDecayTimeMSScaleConverter>(), 0.4, 100, 1000));
}

int MacroControlsGroup::modSrcToParamNumber(MacroControls src)
{
  switch(src)
  {
    case MacroControls::MC1:
      return 243;

    case MacroControls::MC2:
      return 244;

    case MacroControls::MC3:
      return 245;

    case MacroControls::MC4:
      return 246;

    case MacroControls::MC5:
      return 369;

    case MacroControls::MC6:
      return 371;

    default:
      return -1;
  }
}

MacroControls MacroControlsGroup::paramIDToModSrc(int pid)
{
  switch(pid)
  {
    case 243:
      return MacroControls::MC1;

    case 244:
      return MacroControls::MC2;

    case 245:
      return MacroControls::MC3;

    case 246:
      return MacroControls::MC4;

    case 369:
      return MacroControls::MC5;

    case 371:
      return MacroControls::MC6;

    default:
      return MacroControls::NONE;
  }
}
