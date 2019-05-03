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

void RecallEditBufferSerializer::writeTagContent(Writer &writer) const
{
  for(auto &paramGroup : m_editBuffer->m_recallSet.m_parameters)
  {
    RecallParameterSerializer serializer(paramGroup.second.get());
    serializer.write(writer, Attribute("id", paramGroup.first));
  }
}

void RecallEditBufferSerializer::readTagContent(Reader &reader) const
{
  reader.onTag(RecallParameterSerializer::getTagName(), [&](const Attributes &attr) mutable {
    auto id = std::stoi(attr.get("id"));
    auto group = std::make_unique<RecallParameter>(&m_editBuffer->m_recallSet, id);
    auto serializer = new RecallParameterSerializer(group.get());
    m_editBuffer->m_recallSet.m_parameters.at(id) = std::move(group);
    return serializer;
  });
}