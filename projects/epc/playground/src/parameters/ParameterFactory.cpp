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
#include "groups/MonoGroup.h"
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

Parameter* ParameterFactory::createParameterByType(ParameterGroup* parent, int id, VoiceGroup vg)
{
  if(id == C15::PID::Unison_Voices)
    return new UnisonVoicesParameter(parent, vg);
  else if(id == C15::PID::Split_Split_Point)
    return new SplitPointParameter(parent, { id, vg });
  else if(id == ScaleGroup::getScaleBaseParameterNumber())
    return new BaseScaleParameter(parent, { id, vg });
  else if(ScaleGroup::isScaleOffsetParameter(id))
    return new ScaleParameter(parent, { id, vg });
  else if(VoiceGroupMasterGroup::isVoiceGroupMasterParameter(id))
  {
    if(isModulateable(id))
      return new VoiceGroupMasterModulateableParameter(parent, { id, vg });
    else
      return new VoiceGroupMasterUnmodulateableParameter(parent, { id, vg });
  }
  else if(MonoGroup::isMonoParameter({ id, vg }))
  {
    if(isModulateable(id))
      return new ModulateableMonoParameter(parent, { id, vg });
    else
      return new UnmodulateableMonoParameter(parent, { id, vg });
  }
  else if(isModulateable(id))
    return new ModulateableParameter(parent, { id, vg });
  else
    return new Parameter(parent, { id, vg });
}
