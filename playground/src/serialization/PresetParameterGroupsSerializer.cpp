#include "PresetParameterGroupsSerializer.h"
#include "PresetParameterGroupSerializer.h"
#include <presets/Preset.h>
#include <presets/PresetParameterGroup.h>

PresetParameterGroupsSerializer::PresetParameterGroupsSerializer(Preset* preset)
    : Serializer(getTagName())
    , m_preset(preset)
{
}

Glib::ustring PresetParameterGroupsSerializer::getTagName()
{
  return "parameter-groups";
}

void PresetParameterGroupsSerializer::writeTagContent(Writer& writer) const
{
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    for(auto& paramGroup : m_preset->m_parameterGroups[static_cast<int>(vg)])
    {
      PresetParameterGroupSerializer group(paramGroup.second.get());
      group.write(writer, Attribute("id", paramGroup.first));
    }
  }
}

void PresetParameterGroupsSerializer::readTagContent(Reader& reader) const
{
  reader.onTag(PresetParameterGroupSerializer::getTagName(), [&](const Attributes& attr) mutable {
    auto group = m_preset->findParameterGroup(GroupId(attr.get("id")));
    return new PresetParameterGroupSerializer(group);
  });
}