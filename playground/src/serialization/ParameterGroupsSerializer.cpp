#include "xml/Attributes.h"
#include "xml/Writer.h"
#include "xml/Reader.h"
#include "presets/ParameterDualGroupSet.h"
#include "ParameterGroupsSerializer.h"
#include "ParameterGroupSerializer.h"
#include <groups/ParameterGroup.h>

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
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    for(auto paramGroup : m_paramGroups->getParameterGroups(vg))
    {
      ParameterGroupSerializer group(paramGroup);
      group.write(writer, Attribute("id", paramGroup->getID()));
    }
  }
}

void ParameterGroupsSerializer::readTagContent(Reader &reader) const
{
  reader.onTag(ParameterGroupSerializer::getTagName(), [&](auto attr) mutable {
    auto group = m_paramGroups->getParameterGroupByID(GroupId(attr.get("id")));
    return new ParameterGroupSerializer(group);
  });
}
