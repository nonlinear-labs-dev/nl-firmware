#include <presets/EditBuffer.h>
#include "RecallEditBufferSerializer.h"
#include "PresetParameterGroupSerializer.h"
#include "RecallParameterSerializer.h"

RecallEditBufferSerializer::RecallEditBufferSerializer(EditBuffer *edit)
    : Serializer(getTagName())
    , m_editBuffer(edit)
{
}

Glib::ustring RecallEditBufferSerializer::getTagName()
{
  return "recall-parameters";
}

namespace Detail
{
  class RecallEditBufferSerializer2 : public Serializer
  {
   public:
    RecallEditBufferSerializer2(EditBuffer *eb, VoiceGroup vg)
        : Serializer(toString(vg))
        , m_editBuffer{ eb }
        , m_voiceGroup{ vg }
    {
    }

   protected:
    void writeTagContent(Writer &writer) const override
    {
      auto &parameters = m_editBuffer->getRecallParameterSet().getParameters(m_voiceGroup);
      for(auto &param : parameters)
      {
        RecallParameterSerializer serializer(param.second.get());
        serializer.write(writer, Attribute("id", param.first));
      }
    }

    void readTagContent(Reader &reader) const override
    {
      reader.onTag(RecallParameterSerializer::getTagName(), [&, this](const Attributes &attr) mutable {
        auto id = std::stoi(attr.get("id"));
        auto serializer = new RecallParameterSerializer(
            m_editBuffer->getRecallParameterSet().getParameters(m_voiceGroup).at(id).get());
        return serializer;
      });
    }

    EditBuffer *m_editBuffer;
    VoiceGroup m_voiceGroup;
  };
}

void RecallEditBufferSerializer::writeTagContent(Writer &writer) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    Detail::RecallEditBufferSerializer2 ser(m_editBuffer, vg);
    ser.write(writer);
  }
}

void RecallEditBufferSerializer::readTagContent(Reader &reader) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    reader.onTag(toString(vg), [this, vg](const Attributes &attr) mutable {
      return new Detail::RecallEditBufferSerializer2(m_editBuffer, vg);
    });
  }
}
