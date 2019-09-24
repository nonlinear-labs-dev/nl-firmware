#include "VoiceGroupLockSerializer.h"

namespace Detail
{
  class VoiceGroupLockSerializer2 : public Serializer
  {
   public:
    VoiceGroupLockSerializer2(EditBuffer *eb, VoiceGroup vg)
        : Serializer("locks-"+toString(vg))
        , m_editBuffer{ eb }
        , m_vg{ vg }
    {
    }

   protected:
    void writeTagContent(Writer &writer) const override
    {
      for(auto g : m_editBuffer->getParameterGroups(m_vg))
        for(auto p : g->getParameters())
          if(p->isLocked())
            writer.writeTextElement("locked-parameter", to_string(p->getID()));
    }

    void readTagContent(Reader &reader) const override
    {
      reader.onTextElement("locked-parameter", [&](auto text, auto) mutable {
        m_editBuffer->findParameterByID(std::stoi(text), m_vg)->undoableLock(reader.getTransaction());
      });
    }

   protected:
    EditBuffer *m_editBuffer;
    VoiceGroup m_vg;
  };
}

void VoiceGroupLockSerializer::writeTagContent(Writer &writer) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    Detail::VoiceGroupLockSerializer2 ser(m_editBuffer, vg);
    ser.write(writer);
  }
}

void VoiceGroupLockSerializer::readTagContent(Reader &reader) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    reader.onTag("locks-"+toString(vg), [vg, this](const auto attr) mutable {
      return new Detail::VoiceGroupLockSerializer2(m_editBuffer, vg);
    });
  }
}

std::string VoiceGroupLockSerializer::getTagName()
{
  return "locked-parameters";
}

VoiceGroupLockSerializer::VoiceGroupLockSerializer(EditBuffer *eb)
    : Serializer(getTagName())
    , m_editBuffer{ eb }
{
}
