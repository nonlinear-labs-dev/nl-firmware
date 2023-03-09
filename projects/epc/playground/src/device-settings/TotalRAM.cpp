#include <nltools/system/SpawnCommandLine.h>
#include <tools/StringTools.h>
#include "TotalRAM.h"
#include "Settings.h"
#include "nltools/Assert.h"
#include <xml/Writer.h>

Glib::ustring TotalRAM::getDisplayString() const
{
  return m_display;
}

TotalRAM::TotalRAM(DeviceInformation* s)
    : DeviceInformationItem(s)
{
  static SpawnCommandLine cmd("free --mega");
  const auto& out = cmd.getStdOutput();
  auto lines = StringTools::splitStringOnAnyDelimiter(out, '\n');
  auto memory = lines[1];
  auto memoryStats = StringTools::splitStringAtSpacesAndTrimSpaces(memory);
  auto total = memoryStats[1];
  m_display = total;
}

Glib::ustring TotalRAM::get() const
{
  return m_display;
}

void TotalRAM::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("total-ram", get());
}
