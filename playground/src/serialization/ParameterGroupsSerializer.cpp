#include "xml/Attributes.h"
#include "xml/Writer.h"
#include "xml/Reader.h"
#include "presets/ParameterDualGroupSet.h"
#include "ParameterGroupsSerializer.h"
#include "ParameterGroupSerializer.h"

ParameterGroupsSerializer::ParameterGroupsSerializer(ParameterDualGroupSet *paramGroups)
    : Serializer(getTagName())
    , m_paramGroups(paramGroups)
{
}

Glib::ustring ParameterGroupsSerializer::getTagName()
{
  return "parameter-groups";
}

void ParameterGroupsSerializer::writeTagContent(Writer &writer) const
{
  writer.writeTag(toString(VoiceGroup::I), [&writer, this] {
    for(auto paramGroup : m_paramGroups->getParameterGroups(VoiceGroup::I))
    {
      ParameterGroupSerializer group(paramGroup);
      group.write(writer, Attribute("id", paramGroup->getID()));
    }
  });

  writer.writeTag(toString(VoiceGroup::II), [&writer, this] {
    for(auto paramGroup : m_paramGroups->getParameterGroups(VoiceGroup::II))
    {
      ParameterGroupSerializer group(paramGroup);
      group.write(writer, Attribute("id", paramGroup->getID()));
    }
  });
}

void ParameterGroupsSerializer::readTagContent(Reader &reader) const
{
  reader.onTag(toString(VoiceGroup::I), [&](const Attributes& attr) mutable {
    auto group = m_paramGroups->getParameterGroupByID(attr.get("id"), VoiceGroup::I);
    return new ParameterGroupSerializer(group);
  });

  reader.onTag(toString(VoiceGroup::II), [&](const Attributes& attr) mutable {
    auto group = m_paramGroups->getParameterGroupByID(attr.get("id"), VoiceGroup::II);
    return new ParameterGroupSerializer(group);
  });
}
