#include "xml/Attributes.h"

#include "xml/Writer.h"
#include "xml/Reader.h"
#include "presets/PresetManager.h"
#include <presets/Bank.h>
#include "PresetBankOrderSerializer.h"
#include <device-settings/DebugLevel.h>

PresetBankOrderSerializer::PresetBankOrderSerializer(PresetManager *pm)
    : Serializer(getTagName())
    , m_pm(pm)
{
}

Glib::ustring PresetBankOrderSerializer::getTagName()
{
  return "bank-order";
}

void PresetBankOrderSerializer::writeTagContent(Writer &writer) const
{
  m_pm->forEachBank([&](auto bank) { writer.writeTextElement("uuid", bank->getUuid().raw()); });
}

void PresetBankOrderSerializer::readTagContent(Reader &reader) const
{
  reader.onTextElement("uuid", [&](const Glib::ustring &text, auto) mutable {
    auto bank = m_pm->addBank(reader.getTransaction());
    bank->setUuid(reader.getTransaction(), Uuid(text));
  });
}
