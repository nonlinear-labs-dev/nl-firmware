#include "PresetBankSerializer.h"
#include "PresetSerializer.h"

#include <presets/Bank.h>
#include <tools/TimeTools.h>
#include <presets/PresetManager.h>
#include <device-settings/Settings.h>
#include <device-settings/DateTimeAdjustment.h>
#include <presets/EditBuffer.h>
#include <iostream>

PresetBankSerializer::PresetBankSerializer(Bank *bank, Progress progress, bool ignoreUUIDs)
    : super(bank, progress, ignoreUUIDs)
{
}

void PresetBankSerializer::writeTagContent(Writer &writer) const
{
  addStatus("Writing bank " + m_bank->getName(true));

  auto pm  = m_bank->getPresetManager();
  if(pm)
  {
    auto& settings = pm->getEditBuffer()->getSettings();
    writer.writeTextElement("bank-serialize-date", TimeTools::getAdjustedIso(settings.getSetting<DateTimeAdjustment>()));
  }
  else
  {
    writer.writeTextElement("bank-serialize-date", TimeTools::getRealIso());
  }


  super::writeTagContent(writer);

  m_bank->forEachPreset([&, i = 0](auto p) mutable {
    PresetSerializer presetWriter(p);
    presetWriter.write(writer, Attribute("pos", i++));
  });
}

void PresetBankSerializer::readTagContent(Reader &reader) const
{
  const auto bankName = m_bank->getName(false);

  if(!bankName.empty() && bankName != "New Bank")
    addStatus("Reading bank " + bankName);

  super::readTagContent(reader);

  reader.onTag(PresetSerializer::getTagName(), [&](const auto &attr) mutable {
    auto pos = std::stoull(attr.get("pos", to_string(std::numeric_limits<size_t>::max())));
    if(pos < m_bank->getNumPresets())
      return new PresetSerializer(m_bank->getPresetAt(pos), m_ignoreUUIDs);
    auto p = m_bank->appendPreset(reader.getTransaction());
    return new PresetSerializer(p, m_ignoreUUIDs);
  });
}
