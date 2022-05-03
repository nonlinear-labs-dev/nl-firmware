#include <Application.h>
#include "UsedRAM.h"
#include <giomm.h>
#include <nltools/system/SpawnCommandLine.h>
#include <http/UndoScope.h>
#include <nltools/system/SpawnAsyncCommandLine.h>
#include <nltools/logging/Log.h>
#include <tools/StringTools.h>
#include <device-settings/Settings.h>

UsedRAM::UsedRAM(UpdateDocumentContributor& parent)
    : Setting(parent)
    , m_scheduleThrottler { Expiration::Duration { std::chrono::minutes { 1 } } }
{
}

void UsedRAM::init()
{
  if(Application::exists())
  {
    Application::get().getUndoScope()->onUndoScopeChanged(sigc::mem_fun(this, &UsedRAM::scheduleReload));
  }
}

void UsedRAM::load(const Glib::ustring& text, Initiator initiator)
{
  m_displayString = text;
}

Glib::ustring UsedRAM::save() const
{
  return m_displayString;
}

Glib::ustring UsedRAM::getDisplayString() const
{
  return m_displayString;
}

bool UsedRAM::persistent() const
{
  return false;
}

void UsedRAM::scheduleReload()
{
  m_scheduleThrottler.doTask([&] {
    SpawnAsyncCommandLine::spawn(
        std::vector<std::string> { "free", "--mega" },
        [](const std::string& s) {
          if(Application::exists())
          {
            auto lines = StringTools::splitStringOnAnyDelimiter(s, '\n');
            auto memory = lines[1];
            auto memoryStats = StringTools::splitStringAtSpacesAndTrimSpaces(memory);
            auto used = memoryStats[2];
            if(auto setting = Application::get().getSettings()->getSetting<UsedRAM>())
              setting->load(used, Initiator::EXPLICIT_LOAD);
          }
          else {
            nltools::Log::error("Nasty Async Bug Happened!");
          }
        },
        [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
  });
}
