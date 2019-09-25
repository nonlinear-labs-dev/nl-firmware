#include <presets/EditBuffer.h>
#include "VoiceGroupSerializer.h"
#include "ParameterGroupSerializer.h"

namespace Detail
{
  class VoiceGroupSerializer2 : public Serializer
  {
   public:
    VoiceGroupSerializer2(EditBuffer *eb, VoiceGroup vg)
        : Serializer(getTagName())
        , m_editBuffer{ eb }
        , m_voiceGroup{ vg }
    {
    }
    static std::string getTagName()
    {
      return "param-groups";
    }

   protected:
    void writeTagContent(Writer &writer) const override
    {
      for(auto paramGroup : m_editBuffer->getParameterGroups(m_voiceGroup))
      {
        ParameterGroupSerializer group(paramGroup);
        group.write(writer, Attribute("id", paramGroup->getID()));
      }
    }

    void readTagContent(Reader &reader) const override
    {
      reader.onTag(ParameterGroupSerializer::getTagName(), [this](const auto attr) mutable {
        auto group = m_editBuffer->getParameterGroupByID(attr.get("id"), m_voiceGroup);
        return new ParameterGroupSerializer(group);
      });
    }

   private:
    EditBuffer *m_editBuffer;
    VoiceGroup m_voiceGroup;
  };
}

VoiceGroupSerializer::VoiceGroupSerializer(ParameterDualGroupSet *paramGroups)
    : Serializer(getTagName())
    , m_paramGroups{ paramGroups }
{
}

Glib::ustring VoiceGroupSerializer::getTagName()
{
  return "voice-groups";
}

void VoiceGroupSerializer::writeTagContent(Writer &writer) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    writer.writeTag(toString(vg), [vg, &writer, this] {
      Detail::VoiceGroupSerializer2 s(dynamic_cast<EditBuffer *>(m_paramGroups), vg);
      s.write(writer);
    });
  }
}

void VoiceGroupSerializer::readTagContent(Reader &reader) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    reader.onTag(toString(vg), [this, vg](const auto &attr) mutable {
      return new Detail::VoiceGroupSerializer2(dynamic_cast<EditBuffer *>(m_paramGroups), vg);
    });
  }
}
