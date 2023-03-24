#include "PresetPresenter.h"
#include "groups/ScaleGroup.h"
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <parameter_descriptor.h>

bool PresetPresenter::isMonoActive(const Preset *p)
{
  auto monoIEnabled = p->findParameterByID({ C15::PID::Mono_Grp_Enable, VoiceGroup::I }, false);
  auto monoIIEnabled = p->findParameterByID({ C15::PID::Mono_Grp_Enable, VoiceGroup::II }, false);

  if(monoIEnabled && monoIIEnabled)
  {
    return monoIEnabled->getValue() != 0 || monoIIEnabled->getValue() != 0;
  }
  return false;
}

bool PresetPresenter::isUnisonActive(const Preset *p)
{
  auto unisonIEnabled = p->findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::I }, false);
  auto unisonIIEnabled = p->findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::II }, false);

  if(unisonIEnabled && unisonIIEnabled)
  {
    return unisonIEnabled->getValue() > 0 || unisonIIEnabled->getValue() > 0;
  }
  return false;
}

bool PresetPresenter::isScaleActive(const Preset *p)
{
  if(auto scale = p->findParameterGroup({ "Scale", VoiceGroup::Global }))
  {
    for(const auto &scalePresetParameter : scale->getParameters())
      if(scalePresetParameter->getID().getNumber() != ScaleGroup::getScaleBaseParameterNumber())
        if(scalePresetParameter->getValue() != 0)
          return true;
  }
  return false;
}