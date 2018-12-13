#include "AttributesOwnerSerializer.h"
#include <presets/AttributesOwner.h>

#include "xml/Attributes.h"
#include "xml/Writer.h"
#include "xml/Reader.h"

AttributesOwnerSerializer::AttributesOwnerSerializer(AttributesOwner *owner)
    : Serializer(getTagName())
    , m_owner(owner)
{
}

Glib::ustring AttributesOwnerSerializer::getTagName()
{
  return "attributes";
}

void AttributesOwnerSerializer::readProlog(Reader &reader) const
{
  m_owner->clear(reader.getTransaction());
}

void AttributesOwnerSerializer::writeTagContent(Writer &writer) const
{
  for(const auto &a : m_owner->m_attributes)
  {
    writer.writeTextElement("attribute", a.second, Attribute("name", a.first));
  }
}

void AttributesOwnerSerializer::readTagContent(Reader &reader) const
{
  reader.onTextElement("attribute", [&](auto text, auto attr) mutable {
    m_owner->setAttribute(reader.getTransaction(), attr.get("name"), text);
  });
}
