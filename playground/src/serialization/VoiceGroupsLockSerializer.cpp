#include "VoiceGroupsLockSerializer.h"

namespace Detail
{
  template <VoiceGroup VG> class VoiceGroupLockSerializer : public Serializer
  {
   public:
    VoiceGroupLockSerializer(EditBuffer *eb)
        : Serializer(getTag())
        , m_editBuffer{ eb }
    {
    }

    static std::string getTag()
    {
      return "locks-" + toString(VG);
    }

   protected:
    void writeTagContent(Writer &writer) const override
    {
      for(auto g : m_editBuffer->getParameterGroups(VG))
        for(auto p : g->getParameters())
          if(p->isLocked())
            writer.writeTextElement("locked-parameter", to_string(p->getID()));
    }

    void readTagContent(Reader &reader) const override
    {
      reader.onTextElement("locked-parameter", [&](auto text, auto) mutable {
        m_editBuffer->findParameterByID(std::stoi(text), VG)->undoableLock(reader.getTransaction());
      });
    }

   protected:
    EditBuffer *m_editBuffer;
  };
}

void VoiceGroupsLockSerializer::writeTagContent(Writer &writer) const
{
  Detail::VoiceGroupLockSerializer<VoiceGroup::I> i(m_editBuffer);
  i.write(writer);

  Detail::VoiceGroupLockSerializer<VoiceGroup::II> ii(m_editBuffer);
  ii.write(writer);
}

void VoiceGroupsLockSerializer::readTagContent(Reader &reader) const
{
  reader.onTag(Detail::VoiceGroupLockSerializer<VoiceGroup::I>::getTag(), [&](const auto attr) mutable {
    return new Detail::VoiceGroupLockSerializer<VoiceGroup::I>(m_editBuffer);
  });

  reader.onTag(Detail::VoiceGroupLockSerializer<VoiceGroup::II>::getTag(), [&](const auto attr) mutable {
    return new Detail::VoiceGroupLockSerializer<VoiceGroup::II>(m_editBuffer);
  });
}

std::string VoiceGroupsLockSerializer::getTagName()
{
  return "locked-parameters";
}

VoiceGroupsLockSerializer::VoiceGroupsLockSerializer(EditBuffer *eb)
    : Serializer(getTagName())
    , m_editBuffer{ eb }
{
}
