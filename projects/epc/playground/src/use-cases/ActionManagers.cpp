#include "ActionManagers.h"

#include <presets/PresetManagerActions.h>
#include <presets/BankActions.h>
#include <presets/EditBufferActions.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUIActions.h>

ActionManagers::ActionManagers(UpdateDocumentContributor* parent, PresetManager& pm, AudioEngineProxy& aeProx,
                               HWUI& hwui)
{
  m_actionManagers.emplace_back(new PresetManagerActions(parent, pm));
  m_actionManagers.emplace_back(new BankActions(parent, pm));
  m_actionManagers.emplace_back(new EditBufferActions(parent, *pm.getEditBuffer(), aeProx));
  m_actionManagers.emplace_back(new HWUIActions(parent, hwui, *pm.getEditBuffer()));
}

std::list<ActionManagers::tManagerPtr>& ActionManagers::getManagers()
{
  return m_actionManagers;
}
