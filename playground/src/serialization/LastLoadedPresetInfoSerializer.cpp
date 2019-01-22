#include "../xml/Attributes.h"
#include "LastLoadedPresetInfoSerializer.h"
#include "presets/EditBuffer.h"
#include "ParameterGroupsSerializer.h"
#include "xml/Reader.h"
#include "xml/Writer.h"

LastLoadedPresetInfoSerializer::LastLoadedPresetInfoSerializer(EditBuffer *editBuffer)
    : Serializer(getTagName())
    , m_editBuffer(editBuffer)
{
}

Glib::ustring LastLoadedPresetInfoSerializer::getTagName()
{
  return "last-loaded-preset-info";
}

void LastLoadedPresetInfoSerializer::writeTagContent(Writer &writer) const
{
  writer.writeTextElement("loaded-preset", m_editBuffer->getUUIDOfLastLoadedPreset().raw());
  writer.writeTextElement("loaded-presets-name", m_editBuffer->getName());
}

void LastLoadedPresetInfoSerializer::readTagContent(Reader &reader) const
{
  reader.onTextElement("loaded-preset", [&](auto txt, auto) { m_editBuffer->m_lastLoadedPreset = txt; });
  reader.loadTextElement("loaded-presets-name", m_editBuffer->m_name);
}
