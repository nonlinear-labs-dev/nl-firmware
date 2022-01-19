#include "SplitPointEditMenu.h"
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <Application.h>
#include <sigc++/adaptors/hide.h>
#include <device-settings/SyncSplitSettingUseCases.h>

void SplitPointEditMenu::addActions()
{
  ParameterEditButtonMenu::addActions();

  if(m_syncSetting->get())
  {
    addButton("Unlink Splitp.", [this]() {
      auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
      auto pm = Application::get().getPresetManager();
      SyncSplitSettingUseCases useCase(*setting, *pm);
      useCase.disableSyncSetting();
    });
  }
  else
  {
    addButton("Link Splitp.", [this]() {
      auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
      auto pm = Application::get().getPresetManager();
      SyncSplitSettingUseCases useCase(*setting, *pm);
      useCase.enableSyncSetting();
    });
  }
}

SplitPointEditMenu::SplitPointEditMenu(const Rect& rect)
    : ParameterEditButtonMenu(rect)
{
  m_syncSetting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();

  m_syncConnection = Application::get().getSettings()->getSetting<SplitPointSyncParameters>()->onChange(
      sigc::hide(sigc::mem_fun(this, &SplitPointEditMenu::setup)));
}
