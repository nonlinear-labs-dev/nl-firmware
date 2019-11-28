#warning "obsolete"
#if 0
#include <presets/EditBuffer.h>
#include "VoiceGroupsSerializer.h"
#include "ParameterGroupSerializer.h"

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
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    writer.writeTag(toString(vg), [vg, &writer, this] {
      Detail::VoiceGroupSerializer s(m_editBuffer, vg);
      s.write(writer);
    });
  }
}

void VoiceGroupsSerializer::readTagContent(Reader &reader) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    reader.onTag(toString(vg),
                 [this, vg](const auto &attr) mutable { return new Detail::VoiceGroupSerializer(m_editBuffer, vg); });
  }

  reader.onTag(Detail::GlobalParameterGroupsSerializer::tagName(),
               [this](const auto &atttr) mutable { return new Detail::GlobalParameterGroupsSerializer(m_editBuffer); });
}
#endif