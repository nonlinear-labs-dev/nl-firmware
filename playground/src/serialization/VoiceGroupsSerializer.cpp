#include <presets/EditBuffer.h>
#include "VoiceGroupsSerializer.h"
#include "ParameterGroupSerializer.h"

namespace Detail
{
  class VoiceGroupSerializer2 : public Serializer
  {
   public:
    VoiceGroupSerializer2(EditBuffer *eb, VoiceGroup vg)
        : Serializer(tagName())
        , m_editBuffer{ eb }
        , m_voiceGroup{ vg }
    {
    }
    static std::string tagName()
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

  class SplitGroupSerializer : public Serializer
  {
   public:
    SplitGroupSerializer(EditBuffer *eb)
        : Serializer(tagName())
        , m_editBuffer{ eb }
    {
    }

    static std::string tagName()
    {
      return "split-group";
    }

   protected:
    void writeTagContent(Writer &writer) const override
    {
      ParameterGroupSerializer ser(m_editBuffer->getSplitSoundParameterGroup());
      ser.write(writer);
    }

    void readTagContent(Reader &reader) const override
    {
      reader.onTag(ParameterGroupSerializer::getTagName(), [this](const auto &attr) mutable {
        return new ParameterGroupSerializer(m_editBuffer->getSplitSoundParameterGroup());
      });
    }

    EditBuffer *m_editBuffer;
  };
}

VoiceGroupsSerializer::VoiceGroupsSerializer(EditBuffer *editBuffer)
    : Serializer(getTagName())
    , m_editBuffer{ editBuffer }
{
}

Glib::ustring VoiceGroupsSerializer::getTagName()
{
  return "voice-groups";
}

void VoiceGroupsSerializer::writeTagContent(Writer &writer) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    writer.writeTag(toString(vg), [vg, &writer, this] {
      Detail::VoiceGroupSerializer2 s(m_editBuffer, vg);
      s.write(writer);
    });
  }

  Detail::SplitGroupSerializer splits(m_editBuffer);
  splits.write(writer);
}

void VoiceGroupsSerializer::readTagContent(Reader &reader) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    reader.onTag(toString(vg),
                 [this, vg](const auto &attr) mutable { return new Detail::VoiceGroupSerializer2(m_editBuffer, vg); });
  }

  reader.onTag(Detail::SplitGroupSerializer::tagName(),
               [this](const auto &attr) mutable { return new Detail::SplitGroupSerializer(m_editBuffer); });
}
