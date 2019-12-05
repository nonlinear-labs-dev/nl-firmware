#include <presets/EditBuffer.h>
#include <nltools/logging/Log.h>
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
      auto &parameters = m_editBuffer->getRecallParameterSet().getParameters();
      for(auto &param : parameters)
      {
        auto parameter = param.second.get();
        RecallParameterSerializer serializer(parameter);
        serializer.write(writer, Attribute("id", parameter->getID().toString()));
      }
    }

    void readTagContent(Reader &reader) const override
    {
      reader.onTag(RecallParameterSerializer::getTagName(),
                   [&, this](const Attributes &attr) mutable -> RecallParameterSerializer * {
                     auto id = ParameterId(attr.get("id"));
                     auto &rps = m_editBuffer->getRecallParameterSet();
                     try
                     {
                       if(auto param = rps.findParameterByID(id))
                         return new RecallParameterSerializer(param);
                     }
                     catch(...)
                     {
                       nltools::Log::error("Could not find Recall Parameter with ID", id);
                     }
                     return nullptr;
                   });
    }

    EditBuffer *m_editBuffer;
    VoiceGroup m_voiceGroup;
  };
}

void RecallEditBufferSerializer::writeTagContent(Writer &writer) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
  {
    Detail::RecallEditBufferSerializer2 ser(m_editBuffer, vg);
    ser.write(writer);
  }
}

void RecallEditBufferSerializer::readTagContent(Reader &reader) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
  {
    reader.onTag(toString(vg), [this, vg](const Attributes &attr) mutable {
      return new Detail::RecallEditBufferSerializer2(m_editBuffer, vg);
    });
  }
}
