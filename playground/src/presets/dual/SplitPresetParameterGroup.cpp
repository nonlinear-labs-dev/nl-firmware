#include "SplitPresetParameterGroup.h"
#include "presets/PresetParameter.h"
#include "presets/Preset.h"
#include "presets/EditBuffer.h"

SplitPresetParameterGroup::SplitPresetParameterGroup()
    : PresetParameterGroup()
{
  m_parameters[18700] = std::make_unique<PresetParameter>(18700);
}

SplitPresetParameterGroup::SplitPresetParameterGroup(const SplitPresetParameterGroup* p)
    : PresetParameterGroup()
{
  if(p)
  {
    for(auto& para : p->m_parameters)
    {
      m_parameters[para.first] = std::make_unique<PresetParameter>(*para.second);
    }
  }
}

SplitPresetParameterGroup::SplitPresetParameterGroup(const EditBuffer& eb)
{
  if(auto g = eb.getSplitSoundParameterGroup())
  {
    for(auto& param : g->getParameters())
    {
      m_parameters[param->getID()] = std::make_unique<PresetParameter>(*param);
    }
  }
}
