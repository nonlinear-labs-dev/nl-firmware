#include "PresetManagerMetadataSerializer.h"
#include "PresetBankOrderSerializer.h"
#include "EditBufferSerializer.h"
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>

PresetManagerMetadataSerializer::PresetManagerMetadataSerializer(PresetManager *pm, Progress progress)
    : Serializer(getTagName(), progress)
    , m_pm(pm)
{
}

Glib::ustring PresetManagerMetadataSerializer::getTagName()
{
  return "preset-manager";
}

void PresetManagerMetadataSerializer::writeTagContent(Writer &writer) const
{
  EditBufferSerializer eb(m_pm->getEditBuffer(), getProgressCB());
  eb.write(writer);

  writer.writeTextElement("selected-bank-uuid", m_pm->getSelectedBankUuid().raw());

  PresetBankOrderSerializer bankOrder(m_pm);
  bankOrder.write(writer);
}

void PresetManagerMetadataSerializer::readTagContent(Reader &reader) const
{
  reader.onTextElement("selected-bank-uuid",
                       [&](const auto &text, const auto &)
                       { m_pm->selectBank(reader.getTransaction(), Uuid { text }); });

  reader.onTextElement("selected-midi-bank-uuid",
                       [&](const auto &text, const auto &)
                       { m_pm->selectMidiBank(reader.getTransaction(), Uuid { text }); });

  reader.onTag(EditBufferSerializer::getTagName(),
               [&](const auto &) mutable { return new EditBufferSerializer(m_pm->getEditBuffer(), getProgressCB()); });

  reader.onTag(PresetBankOrderSerializer::getTagName(),
               [&](const auto &) mutable { return new PresetBankOrderSerializer(m_pm); });
}
