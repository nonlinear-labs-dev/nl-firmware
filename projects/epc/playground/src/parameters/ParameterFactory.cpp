#include "ParameterFactory.h"
#include "parameter_group.h"
#include "nltools/Assert.h"
#include "parameter_list.h"
#include "UnisonVoicesParameter.h"
#include "SplitPointParameter.h"
#include "groups/ScaleGroup.h"
#include "ScaleParameter.h"
#include "parameters/voice-group-master-group/VoiceGroupMasterModulateableParameter.h"
#include "groups/VoiceGroupMasterGroup.h"
#include "parameters/mono-mode-parameters/ModulateableMonoParameter.h"
#include "parameters/mono-mode-parameters/UnmodulateableMonoParameter.h"

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

std::vector<int> ParameterFactory::getParameterIDs(const std::string& longName)
{
  if(longName == "Oscillator A")
    return fromArray<C15::Descriptors::ParameterGroup::Osc_A>();
  if(longName == "Oscillator B")
    return fromArray<C15::Descriptors::ParameterGroup::Osc_B>();
  if(longName == "Envelope A")
    return fromArray<C15::Descriptors::ParameterGroup::Env_A>();
  if(longName == "Envelope B")
    return fromArray<C15::Descriptors::ParameterGroup::Env_B>();
  if(longName == "Envelope C")
    return fromArray<C15::Descriptors::ParameterGroup::Env_C>();
  if(longName == "Shaper A")
    return fromArray<C15::Descriptors::ParameterGroup::Shp_A>();
  if(longName == "Shaper B")
    return fromArray<C15::Descriptors::ParameterGroup::Shp_B>();
  if(longName == "FB Mixer")
    return fromArray<C15::Descriptors::ParameterGroup::FB_Mix>();
  if(longName == "Comb Filter")
    return fromArray<C15::Descriptors::ParameterGroup::Comb_Flt>();
  if(longName == "SV Filter")
    return fromArray<C15::Descriptors::ParameterGroup::SV_Flt>();
  if(longName == "Output Mixer")
    return fromArray<C15::Descriptors::ParameterGroup::Out_Mix>();
  if(longName == "Flanger")
    return fromArray<C15::Descriptors::ParameterGroup::Flanger>();
  if(longName == "Cabinet")
    return fromArray<C15::Descriptors::ParameterGroup::Cabinet>();
  if(longName == "Gap Filter")
    return fromArray<C15::Descriptors::ParameterGroup::Gap_Flt>();
  if(longName == "Echo")
    return fromArray<C15::Descriptors::ParameterGroup::Echo>();
  if(longName == "Reverb")
    return fromArray<C15::Descriptors::ParameterGroup::Reverb>();
  if(longName == "Unison")
    return fromArray<C15::Descriptors::ParameterGroup::Unison>();
  if(longName == "Split")
    return fromArray<C15::Descriptors::ParameterGroup::Split>();
  if(longName == "Mono")
    return fromArray<C15::Descriptors::ParameterGroup::Mono_Grp>();
  if(longName == "Part")
    return fromArray<C15::Descriptors::ParameterGroup::Voice_Grp>();
  if(longName == "Master")
    return fromArray<C15::Descriptors::ParameterGroup::Master>();
  if(longName == "Scale")
    return fromArray<C15::Descriptors::ParameterGroup::Scale>();

  nltools_detailedAssertAlways(false, "Unknown parameter group: " + longName);
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
