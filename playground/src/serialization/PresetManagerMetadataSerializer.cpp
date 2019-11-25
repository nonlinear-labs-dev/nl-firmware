#include "PresetManagerMetadataSerializer.h"
#include "PresetBankOrderSerializer.h"
#include "EditBufferSerializer.h"
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>

PresetManagerMetadataSerializer::PresetManagerMetadataSerializer(PresetManager *pm)
    : Serializer(getTagName())
    , m_pm(pm)
{
}

Glib::ustring PresetManagerMetadataSerializer::getTagName()
{
  return "preset-manager";
}

void PresetManagerMetadataSerializer::writeTagContent(Writer &writer) const
{
  writer.writeTextElement("selected-bank-uuid", m_pm->getSelectedBankUuid().raw());

  EditBufferSerializer eb(m_pm->getEditBuffer());
  eb.write(writer);

  PresetBankOrderSerializer bankOrder(m_pm);
  bankOrder.write(writer);
}

void PresetManagerMetadataSerializer::readTagContent(Reader &reader) const
{
  reader.onTextElement("selected-bank-uuid",
                       [&](const auto &text, const auto &) { m_pm->selectBank(reader.getTransaction(), text); });

  reader.onTag(EditBufferSerializer::getTagName(),
               [&](const auto &) mutable { return new EditBufferSerializer(m_pm->getEditBuffer()); });

  reader.onTag(PresetBankOrderSerializer::getTagName(),
               [&](const auto &) mutable { return new PresetBankOrderSerializer(m_pm); });
}
