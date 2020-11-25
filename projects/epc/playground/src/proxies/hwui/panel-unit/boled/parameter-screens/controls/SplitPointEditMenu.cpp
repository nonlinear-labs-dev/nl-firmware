#include "SplitPointEditMenu.h"
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <Application.h>
#include <sigc++/adaptors/hide.h>

void SplitPointEditMenu::addActions()
{
  ParameterEditButtonMenu::addActions();

  if(m_syncSetting->get())
  {
    addButton("Unlink Splitp.", [this]() { m_syncSetting->setState(false); });
  }
  else
  {
    addButton("Link Splitp.", [this]() { m_syncSetting->setState(true); });
  }
}

SplitPointEditMenu::SplitPointEditMenu(const Rect& rect)
    : ParameterEditButtonMenu(rect)
{
  m_syncSetting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>().get();

  m_syncConnection = Application::get().getSettings()->getSetting<SplitPointSyncParameters>()->onChange(
      sigc::hide(sigc::mem_fun(this, &SplitPointEditMenu::setup)));
}
