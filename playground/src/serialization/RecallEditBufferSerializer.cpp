#include <presets/EditBuffer.h>
#include <nltools/logging/Log.h>
#include "RecallEditBufferSerializer.h"
#include "PresetParameterGroupSerializer.h"
#include "RecallParameterSerializer.h"
#include <presets/recall/RecallParameter.h>

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
  auto &parameters = m_editBuffer->getRecallParameterSet().getParameters();
  for(auto &param : parameters)
  {
    auto parameter = param.second.get();
    RecallParameterSerializer serializer(parameter);
    serializer.write(writer, Attribute("id", parameter->getID().toString()));
  }
}

void RecallEditBufferSerializer::readTagContent(Reader &reader) const
{
  reader.onTag(RecallParameterSerializer::getTagName(), [&, this](const Attributes &attr) {
    auto id = ParameterId(attr.get("id"));
    auto &rps = m_editBuffer->getRecallParameterSet();
    if(auto param = rps.findParameterByID(id))
      return new RecallParameterSerializer(param);
    return static_cast<RecallParameterSerializer *>(nullptr);
  });
}
