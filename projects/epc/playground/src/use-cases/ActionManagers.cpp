#include "ActionManagers.h"

#include <presets/PresetManagerActions.h>
#include <presets/BankActions.h>
#include <presets/EditBufferActions.h>
#include <presets/PresetManager.h>

#include <proxies/hwui/HWUIActions.h>

#include <device-settings/Settings.h>
#include <device-settings/SettingsActions.h>

ActionManagers::ActionManagers(UpdateDocumentContributor* parent, PresetManager& pm, AudioEngineProxy& aeProx,
                               HWUI& hwui, Settings& settings, VoiceGroupAndLoadToPartManager& vgManager)
{
  m_actionManagers.emplace_back(new SettingsActions(parent, settings, pm));
  m_actionManagers.emplace_back(new PresetManagerActions(parent, pm, aeProx, settings));
  m_actionManagers.emplace_back(new BankActions(parent, pm, settings));
  m_actionManagers.emplace_back(new EditBufferActions(parent, *pm.getEditBuffer(), aeProx, settings));
  m_actionManagers.emplace_back(new HWUIActions(parent, hwui, *pm.getEditBuffer(), vgManager));
}

std::list<ActionManagers::tManagerPtr>& ActionManagers::getManagers()
{
  return m_actionManagers;
}
