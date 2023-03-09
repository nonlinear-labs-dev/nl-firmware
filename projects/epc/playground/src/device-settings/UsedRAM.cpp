#include <Application.h>
#include "UsedRAM.h"
#include <giomm.h>
#include <nltools/system/SpawnCommandLine.h>
#include <http/UndoScope.h>
#include <nltools/system/SpawnAsyncCommandLine.h>
#include <nltools/logging/Log.h>
#include <tools/StringTools.h>
#include <device-settings/Settings.h>
#include <xml/Writer.h>

UsedRAM::UsedRAM(DeviceInformation* parent)
    : DeviceInformationItem(parent)
    , m_scheduleThrottler(Application::get().getMainContext(), Expiration::Duration { std::chrono::minutes { 1 } })
{
  Application::get().getUndoScope()->onUndoScopeChanged(sigc::mem_fun(this, &UsedRAM::scheduleReload));
}

void UsedRAM::load(const Glib::ustring& text)
{
  m_displayString = text;
}

Glib::ustring UsedRAM::getDisplayString() const
{
  return m_displayString;
}

void UsedRAM::scheduleReload()
{
  m_scheduleThrottler.doTask(
      [&]
      {
        SpawnAsyncCommandLine::spawn(
            Application::get().getMainContext(), std::vector<std::string> { "free", "--mega" },
            [](const std::string& s)
            {
              if(Application::exists())
              {
                auto lines = StringTools::splitStringOnAnyDelimiter(s, '\n');
                auto memory = lines[1];
                auto memoryStats = StringTools::splitStringAtSpacesAndTrimSpaces(memory);
                auto used = memoryStats[2];
                if(auto usedRam = Application::get().getDeviceInformation()->getItem<UsedRAM>())
                  usedRam->load(used);
              }
              else
              {
                nltools::Log::error("Nasty Async Bug Happened!");
              }
            },
            [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
      });
}

Glib::ustring UsedRAM::get() const
{
  return m_displayString;
}

void UsedRAM::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("used-ram", get());
}
