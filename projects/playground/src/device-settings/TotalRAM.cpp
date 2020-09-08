#include <nltools/system/SpawnCommandLine.h>
#include <tools/StringTools.h>
#include "TotalRAM.h"
#include "Settings.h"

void TotalRAM::load(const Glib::ustring& text)
{
  m_display = text;
}

Glib::ustring TotalRAM::save() const
{
  return m_display;
}

Glib::ustring TotalRAM::getDisplayString() const
{
  return m_display;
}

bool TotalRAM::persistent() const
{
  return false;
}

TotalRAM::TotalRAM(Settings& s)
    : Setting(s)
{
  static SpawnCommandLine cmd("free --mega");
  const auto& out = cmd.getStdOutput();
  auto lines = StringTools::splitStringOnAnyDelimiter(out, '\n');
  auto memory = lines[1];
  auto memoryStats = StringTools::splitStringAtSpacesAndTrimSpaces(memory);
  auto total = memoryStats[1];

  load(total);
}
