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
        auto &rps = m_editBuffer->getRecallParameterSet();
        try
        {
          auto param = rps.getParameters(m_voiceGroup).at(id).get();
          auto serializer = new RecallParameterSerializer(param);
          return serializer;
        }
        catch(...)
        {
          return static_cast<RecallParameterSerializer *>(nullptr);
        }
      });
    }

    EditBuffer *m_editBuffer;
    VoiceGroup m_voiceGroup;
  };

  class GlobalRecallSerializer : public Serializer
  {
   public:
    GlobalRecallSerializer(EditBuffer *eb)
        : Serializer(tagName())
        , m_editBuffer(eb)
    {
    }

    static std::string tagName()
    {
      return "GlobalRecall";
    }

   protected:
    void writeTagContent(Writer &writer) const override
    {
      auto &parameters = m_editBuffer->getRecallParameterSet().getGlobalParameters();
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
        auto &rps = m_editBuffer->getRecallParameterSet();
        try
        {
          auto param = rps.findParameterByID(id, VoiceGroup::Global);
          auto serializer = new RecallParameterSerializer(param);
          return serializer;
        }
        catch(...)
        {
          return static_cast<RecallParameterSerializer *>(nullptr);
        }
      });
    }

   private:
    EditBuffer *m_editBuffer;
  };
}

void RecallEditBufferSerializer::writeTagContent(Writer &writer) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    Detail::RecallEditBufferSerializer2 ser(m_editBuffer, vg);
    ser.write(writer);
  }

  Detail::GlobalRecallSerializer ser(m_editBuffer);
  ser.write(writer);
}

void RecallEditBufferSerializer::readTagContent(Reader &reader) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    reader.onTag(toString(vg), [this, vg](const Attributes &attr) mutable {
      return new Detail::RecallEditBufferSerializer2(m_editBuffer, vg);
    });
  }

  reader.onTag(Detail::GlobalRecallSerializer::tagName(),
               [this](const Attributes &attr) mutable { return new Detail::GlobalRecallSerializer(m_editBuffer); });
}
