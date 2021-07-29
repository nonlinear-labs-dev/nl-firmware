#include "PresetBankSerializer.h"
#include "PresetSerializer.h"

#include <presets/Bank.h>
#include <tools/TimeTools.h>

PresetBankSerializer::PresetBankSerializer(Bank *bank, Progress progress, bool ignoreUUIDs)
    : super(bank, progress, ignoreUUIDs)
{
}

void PresetBankSerializer::writeTagContent(Writer &writer) const
{
  addStatus("Writing bank " + m_bank->getName(true));

  writer.writeTextElement("bank-serialize-date", TimeTools::getAdjustedIso());

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
