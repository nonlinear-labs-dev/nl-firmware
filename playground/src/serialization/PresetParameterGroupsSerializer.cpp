#include "PresetParameterGroupsSerializer.h"
#include "PresetParameterGroupSerializer.h"
#include <presets/Preset.h>
#include <presets/PresetParameterGroup.h>

namespace Detail
{
  class PresetSplitGroupSerializer : public Serializer
  {
   public:
    PresetSplitGroupSerializer(Preset* p)
        : Serializer(getTagName())
        , m_preset{ p }
    {
    }

    static Glib::ustring getTagName()
    {
      return "split-group";
    }

   protected:
    void writeTagContent(Writer& writer) const override
    {
      PresetParameterGroupSerializer ser(m_preset->getSplitGroup());
      ser.write(writer);
    }

    void readTagContent(Reader& reader) const override
    {
      reader.onTag(PresetParameterGroupSerializer::getTagName(), [&](const auto& attr) mutable {
        return new PresetParameterGroupSerializer(m_preset->getSplitGroup());
      });
    }

    Preset* m_preset;
  };
}

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
  writer.writeTag(toString(VoiceGroup::I), [&writer, this] {
    for(auto& paramGroup : m_preset->m_parameterGroups[static_cast<int>(VoiceGroup::I)])
    {
      PresetParameterGroupSerializer group(paramGroup.second.get());
      group.write(writer, Attribute("id", paramGroup.first));
    }
  });

  writer.writeTag(toString(VoiceGroup::II), [&writer, this] {
    for(auto& paramGroup : m_preset->m_parameterGroups[static_cast<int>(VoiceGroup::II)])
    {
      PresetParameterGroupSerializer group(paramGroup.second.get());
      group.write(writer, Attribute("id", paramGroup.first));
    }
  });

  Detail::PresetSplitGroupSerializer ser(m_preset);
  ser.write(writer);
}

void PresetParameterGroupsSerializer::readTagContent(Reader& reader) const
{
  reader.onTag(toString(VoiceGroup::I), [&](const Attributes& attr) mutable {
    auto group = m_preset->findParameterGroup(attr.get("id"), VoiceGroup::I);
    return new PresetParameterGroupSerializer(group);
  });

  reader.onTag(toString(VoiceGroup::II), [&](const Attributes& attr) mutable {
    auto group = m_preset->findParameterGroup(attr.get("id"), VoiceGroup::II);
    return new PresetParameterGroupSerializer(group);
  });

  reader.onTag(Detail::PresetSplitGroupSerializer::getTagName(),
               [&](const Attributes& attr) mutable { return new Detail::PresetSplitGroupSerializer(m_preset); });
}