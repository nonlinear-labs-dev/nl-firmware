#include "ParameterFactory.h"
#include "parameter_group.h"
#include "nltools/Assert.h"
#include "parameter_list.h"
#include "UnisonVoicesParameter.h"
#include "SplitPointParameter.h"
#include "groups/ScaleGroup.h"
#include "ScaleParameter.h"
#include "parameters/voice-group-master-group/VoiceGroupMasterModulateableParameter.h"
#include "parameters/mono-mode-parameters/ModulateableMonoParameter.h"
#include "parameters/mono-mode-parameters/UnmodulateableMonoParameter.h"
#include "MacroControlSmoothingParameter.h"
#include "MacroControlParameter.h"

namespace
{
  template <C15::Descriptors::ParameterGroup tGroup> std::vector<int> fromArray()
  {
    const auto& elements = C15::ParameterGroupElementList<tGroup>::sElements;
    const auto& size = C15::ParameterGroupElementList<tGroup>::sSize;
    std::vector<int> result;
    result.reserve(size);
    for(int i = 0; i < size; i++)
    {
      result.push_back(elements[i]);
    }
    return result;
  }
}

std::vector<int> ParameterFactory::getParameterIDs(const std::string& shortName)
{
  if(shortName == "Oscillator A")
    return fromArray<C15::Descriptors::ParameterGroup::Osc_A>();
  if(shortName == "Oscillator B")
    return fromArray<C15::Descriptors::ParameterGroup::Osc_B>();
  if(shortName == "Envelope A")
    return fromArray<C15::Descriptors::ParameterGroup::Env_A>();
  if(shortName == "Envelope B")
    return fromArray<C15::Descriptors::ParameterGroup::Env_B>();
  if(shortName == "Envelope C")
    return fromArray<C15::Descriptors::ParameterGroup::Env_C>();
  if(shortName == "Shaper A")
    return fromArray<C15::Descriptors::ParameterGroup::Shp_A>();
  if(shortName == "Shaper B")
    return fromArray<C15::Descriptors::ParameterGroup::Shp_B>();
  if(shortName == "FB Mixer")
    return fromArray<C15::Descriptors::ParameterGroup::FB_Mix>();
  if(shortName == "Comb Filter")
    return fromArray<C15::Descriptors::ParameterGroup::Comb_Flt>();
  if(shortName == "SV Filter")
    return fromArray<C15::Descriptors::ParameterGroup::SV_Flt>();
  if(shortName == "Output Mixer")
    return fromArray<C15::Descriptors::ParameterGroup::Out_Mix>();
  if(shortName == "Flanger")
    return fromArray<C15::Descriptors::ParameterGroup::Flanger>();
  if(shortName == "Cabinet")
    return fromArray<C15::Descriptors::ParameterGroup::Cabinet>();
  if(shortName == "Gap Filter")
    return fromArray<C15::Descriptors::ParameterGroup::Gap_Flt>();
  if(shortName == "Echo")
    return fromArray<C15::Descriptors::ParameterGroup::Echo>();
  if(shortName == "Reverb")
    return fromArray<C15::Descriptors::ParameterGroup::Reverb>();
  if(shortName == "Unison")
    return fromArray<C15::Descriptors::ParameterGroup::Unison>();
  if(shortName == "Split")
    return fromArray<C15::Descriptors::ParameterGroup::Split>();
  if(shortName == "Mono")
    return fromArray<C15::Descriptors::ParameterGroup::Mono_Grp>();
  if(shortName == "Part")
    return fromArray<C15::Descriptors::ParameterGroup::Voice_Grp>();
  if(shortName == "Master")
    return fromArray<C15::Descriptors::ParameterGroup::Master>();
  if(shortName == "Scale")
    return fromArray<C15::Descriptors::ParameterGroup::Scale>();
  if(shortName == "Macro Control")
    return fromArray<C15::Descriptors::ParameterGroup::Macro>();

  nltools_detailedAssertAlways(false, ("Unknown parameter group: " + shortName));
}

bool ParameterFactory::isModulateable(int id)
{
  const auto& descriptor = C15::ParameterList[id];
  const auto type = descriptor.m_param.m_type;
  return (type == C15::Descriptors::ParameterType::Polyphonic_Modulateable
          || type == C15::Descriptors::ParameterType::Monophonic_Modulateable
          || type == C15::Descriptors::ParameterType::Global_Modulateable);
}

Parameter* ParameterFactory::createParameterByType(ParameterGroup* parent, const ParameterId& id)
{
  if(id.getNumber() == C15::PID::Unison_Voices)
    return new UnisonVoicesParameter(parent, id.getVoiceGroup());
  else if(id.getNumber() == C15::PID::Split_Split_Point)
    return new SplitPointParameter(parent, id);
  else if(id.getNumber() == ScaleGroup::getScaleBaseParameterNumber())
    return new BaseScaleParameter(parent, id);
  else if(isScaleOffsetParameter(id))
    return new ScaleParameter(parent, id);
  else if(isMacroControl(id))
    return new MacroControlParameter(parent, id);
  else if(isMacroTime(id))
    return new MacroControlSmoothingParameter(parent, id);
  else if(isVoiceGroupMasterParameter(id))
  {
    if(isModulateable(id.getNumber()))
      return new VoiceGroupMasterModulateableParameter(parent, id);
    else
      return new VoiceGroupMasterUnmodulateableParameter(parent, id);
  }
  else if(ParameterFactory::isMonoParameter(id))
  {
    if(isModulateable(id.getNumber()))
      return new ModulateableMonoParameter(parent, id);
    else
      return new UnmodulateableMonoParameter(parent, id);
  }
  else if(isModulateable(id.getNumber()))
    return new ModulateableParameter(parent, id);
  else
    return new Parameter(parent, id);
}

std::vector<C15::ParameterGroupDescriptor> ParameterFactory::getParameterGroupsPerVoiceGroup()
{
  std::vector<C15::ParameterGroupDescriptor> groups;
  for(const auto& group : C15::ParameterGroups)
  {
    if(group.m_group == C15::Descriptors::ParameterGroup::None
       || group.m_group == C15::Descriptors::ParameterGroup::Voice_Grp
       || group.m_group == C15::Descriptors::ParameterGroup::Env_G)
      continue;

    if(group.m_global_group == false)
    {
      groups.push_back(group);
    }
  }
  return groups;
}

bool ParameterFactory::isUnisonParameter(const ParameterId& id)
{
  const auto n = id.getNumber();
  return n == C15::PID::Unison_Voices || n == C15::PID::Unison_Detune || n == C15::PID::Unison_Phase
      || n == C15::PID::Unison_Pan;
}

bool ParameterFactory::isUnisonParameter(const Parameter* parameter)
{
  if(parameter)
    return ParameterFactory::isUnisonParameter(parameter->getID());
  return false;
}

bool ParameterFactory::isSplitPoint(const Parameter* p)
{
  if(p)
    return isSplitPoint(p->getID());
  return false;
}

bool ParameterFactory::isSplitPoint(const ParameterId& id)
{
  return id.getNumber() == C15::PID::Split_Split_Point;
}

bool ParameterFactory::isMonoParameter(const ParameterId& id)
{
  using namespace C15::PID;
  auto number = id.getNumber();
  return number == Mono_Grp_Enable || number == Mono_Grp_Prio || number == Mono_Grp_Legato || number == Mono_Grp_Glide;
}

bool ParameterFactory::isMonoParameter(const Parameter* parameter)
{
  if(parameter)
    return isMonoParameter(parameter->getID());
  return false;
}

bool ParameterFactory::isVoiceGroupMasterParameter(const ParameterId& parameterId)
{
  const auto id = parameterId.getNumber();
  return id == C15::PID::Voice_Grp_Volume || id == C15::PID::Voice_Grp_Tune || id == C15::PID::Voice_Grp_Mute
      || id == C15::PID::Voice_Grp_Fade_From || id == C15::PID::Voice_Grp_Fade_Range;
}

bool ParameterFactory::isMasterParameter(const Parameter* p)
{
  if(p)
  {
    auto id = p->getID().getNumber();
    return id == C15::PID::Master_Volume || id == C15::PID::Master_Tune || id == C15::PID::Master_Serial_FX
        || id == C15::PID::Master_Pan || id == C15::PID::Master_FX_Mix;
  }
  return false;
}

bool ParameterFactory::isScaleParameter(const ParameterId& id)
{
  auto number = id.getNumber();
  return (number >= C15::PID::Scale_Base_Key && number <= C15::PID::Scale_Offset_11)
      || number == C15::PID::Scale_Offset_0;
}

bool ParameterFactory::isScaleOffsetParameter(const ParameterId& id)
{
  auto number = id.getNumber();
  return (number >= C15::PID::Scale_Offset_1 && number <= C15::PID::Scale_Offset_11)
      || number == C15::PID::Scale_Offset_0;
}

bool ParameterFactory::isScaleParameter(const Parameter* parameter)
{
  if(parameter)
    return isScaleParameter(parameter->getID());
  return false;
}

ParameterId ParameterFactory::modSrcToParamId(MacroControls src)
{
  switch(src)
  {
    case MacroControls::MC1:
      return ParameterId(C15::PID::MC_A, VoiceGroup::Global);

    case MacroControls::MC2:
      return ParameterId(C15::PID::MC_B, VoiceGroup::Global);

    case MacroControls::MC3:
      return ParameterId(C15::PID::MC_C, VoiceGroup::Global);

    case MacroControls::MC4:
      return ParameterId(C15::PID::MC_D, VoiceGroup::Global);

    case MacroControls::MC5:
      return ParameterId(C15::PID::MC_E, VoiceGroup::Global);

    case MacroControls::MC6:
      return ParameterId(C15::PID::MC_F, VoiceGroup::Global);

    default:
      break;
  }

  return ParameterId::invalid();
}

MacroControls ParameterFactory::paramIDToModSrc(const ParameterId& pid)
{
  switch(pid.getNumber())
  {
    case C15::PID::MC_A:
      return MacroControls::MC1;

    case C15::PID::MC_B:
      return MacroControls::MC2;

    case C15::PID::MC_C:
      return MacroControls::MC3;

    case C15::PID::MC_D:
      return MacroControls::MC4;

    case C15::PID::MC_E:
      return MacroControls::MC5;

    case C15::PID::MC_F:
      return MacroControls::MC6;

    default:
      return MacroControls::NONE;
  }
}

bool ParameterFactory::isMacroTime(const ParameterId& id)
{
  return (id.getNumber() >= C15::PID::MC_Time_A && id.getNumber() <= C15::PID::MC_Time_D)
      || (id.getNumber() == C15::PID::MC_Time_E || id.getNumber() == C15::PID::MC_Time_F);
}

bool ParameterFactory::isMacroControl(const ParameterId& id)
{
  auto paramNumber = id.getNumber();
  return paramNumber == C15::PID::MC_A || paramNumber == C15::PID::MC_B || paramNumber == C15::PID::MC_C
      || paramNumber == C15::PID::MC_D || paramNumber == C15::PID::MC_E || paramNumber == C15::PID::MC_F;
}

ParameterId ParameterFactory::smoothingIdToMCId(const ParameterId& smoothingId)
{
  switch(smoothingId.getNumber())
  {
    case C15::PID::MC_Time_A:
      return { C15::PID::MC_A, VoiceGroup::Global };
    case C15::PID::MC_Time_B:
      return { C15::PID::MC_B, VoiceGroup::Global };
    case C15::PID::MC_Time_C:
      return { C15::PID::MC_C, VoiceGroup::Global };
    case C15::PID::MC_Time_D:
      return { C15::PID::MC_D, VoiceGroup::Global };
    case C15::PID::MC_Time_E:
      return { C15::PID::MC_E, VoiceGroup::Global };
    case C15::PID::MC_Time_F:
      return { C15::PID::MC_F, VoiceGroup::Global };
    default:
      return ParameterId::invalid();
  }
}
