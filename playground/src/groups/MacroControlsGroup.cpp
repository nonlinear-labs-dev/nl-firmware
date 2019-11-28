#include "MacroControlsGroup.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/scale-converters/EnvelopeAttackDecayTimeMSScaleConverter.h"

MacroControlsGroup::MacroControlsGroup(ParameterDualGroupSet *parent)
    : ParameterGroup(parent, { "MCs", VoiceGroup::Global }, "Macro Control", "Macro Control", "Macro Control")
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

ParameterId MacroControlsGroup::modSrcToParamId(MacroControls src)
{
  switch(src)
  {
    case MacroControls::MC1:
      return ParameterId(243, VoiceGroup::Global);

    case MacroControls::MC2:
      return ParameterId(244, VoiceGroup::Global);

    case MacroControls::MC3:
      return ParameterId(245, VoiceGroup::Global);

    case MacroControls::MC4:
      return ParameterId(246, VoiceGroup::Global);

    case MacroControls::MC5:
      return ParameterId(369, VoiceGroup::Global);

    case MacroControls::MC6:
      return ParameterId(371, VoiceGroup::Global);

    default:
      break;
  }

  nltools::throwException("Unkown macro control id");
  return { -1, VoiceGroup::Global };
}

MacroControls MacroControlsGroup::paramIDToModSrc(ParameterId pid)
{
  switch(pid.getNumber())
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
