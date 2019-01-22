#include "PresetOrderSerializer.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <tools/Uuid.h>

PresetOrderSerializer::PresetOrderSerializer(Bank *bank, bool ignoreUUIDs)
    : Serializer(getTagName())
    , m_bank(bank)
    , m_ignoreUUIDs(ignoreUUIDs)
{
}

Glib::ustring PresetOrderSerializer::getTagName()
{
  return "preset-order";
}

void PresetOrderSerializer::writeTagContent(Writer &writer) const
{
  m_bank->forEachPreset([&](auto p) { writer.writeTextElement("uuid", p->getUuid().raw()); });
}

void PresetOrderSerializer::readTagContent(Reader &reader) const
{
  reader.onTextElement("uuid", [&](auto &text, auto) mutable {
    auto p = m_bank->appendPreset(reader.getTransaction());

    if(!m_ignoreUUIDs)
      p->setUuid(reader.getTransaction(), text);
  });
}
