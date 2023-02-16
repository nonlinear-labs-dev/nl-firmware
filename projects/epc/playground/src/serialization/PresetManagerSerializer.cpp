#include "PresetBankSerializer.h"
#include "PresetManagerSerializer.h"
#include <tools/TimeTools.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/DateTimeAdjustment.h>
#include <iostream>

PresetManagerSerializer::PresetManagerSerializer(PresetManager *pm, Progress progress)
    : Serializer(getTagName(), progress)
    , m_pm(pm)
{
}

PresetManagerSerializer::~PresetManagerSerializer()
{
  m_pm->m_sigRestoreHappened.send();
}

Glib::ustring PresetManagerSerializer::getTagName()
{
  return "preset-manager";
}

void PresetManagerSerializer::writeTagContent(Writer &writer) const
{
  auto adj = m_pm->getEditBuffer()->getSettings().getSetting<DateTimeAdjustment>();
  writer.writeTextElement("serialize-date", TimeTools::getAdjustedIso(adj));
  writer.writeTextElement("selected-bank-uuid", m_pm->getSelectedBankUuid().raw());
  writer.writeTextElement("selected-midi-bank-uuid", m_pm->getMidiSelectedBank().raw());

  addStatus("Writing PresetManager");

  m_pm->forEachBank([&](auto bank) {
    PresetBankSerializer bankWriter(bank, getProgressCB());
    bankWriter.write(writer);
  });
}

void PresetManagerSerializer::readTagContent(Reader &reader) const
{
  addStatus("Reading PresetManager");

  reader.onTextElement("selected-bank-uuid", [&](const auto &text, const auto &) {
    m_pm->selectBank(reader.getTransaction(), Uuid { text });
  });

  reader.onTextElement("selected-midi-bank-uuid", [&](const auto &text, const auto &) {
    m_pm->selectMidiBank(reader.getTransaction(), Uuid { text });
  });

  reader.onTag(PresetBankSerializer::getTagName(), [&](const auto &) mutable {
    return new PresetBankSerializer(m_pm->addBank(reader.getTransaction()), getProgressCB());
  });
}
