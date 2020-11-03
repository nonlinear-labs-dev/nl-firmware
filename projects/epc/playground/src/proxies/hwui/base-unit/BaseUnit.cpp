#include "BaseUnit.h"
#include "BaseUnitPlayMode.h"
#include "BaseUnitEditMode.h"
#include "BaseUnitPresetsMode.h"
#include "BaseUnitBanksMode.h"
#include "Application.h"
#include "device-settings/Settings.h"
#include "device-settings/BaseUnitUIMode.h"
#include "BaseUnitScreenSaverMode.h"
#include <nltools/messaging/Message.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>

BaseUnit::BaseUnit()
{
  auto settings = Application::get().getSettings();
  settings->getSetting<ScreenSaverTimeoutSetting>()->onScreenSaverStateChanged(
      sigc::mem_fun(this, &BaseUnit::onScreenSaverStateChanged));

  settings->getSetting<BaseUnitUIMode>()->onChange(mem_fun(this, &BaseUnit::respectUsageMode));

  nltools::msg::onConnectionEstablished(nltools::msg::EndPoint::RibbonLed,
                                        sigc::mem_fun(this, &BaseUnit::onBBBBConnected));
}

BaseUnit::~BaseUnit() = default;

void BaseUnit::init()
{
  m_playPanel.getSOLED().init();
}

void BaseUnit::onBBBBConnected()
{
  m_upperRibbon.syncBBBB();
  m_lowerRibbon.syncBBBB();
}

void BaseUnit::onScreenSaverStateChanged(bool state)
{
  if(state)
  {
    m_stashedUsageMode = getUsageMode();
    setUsageMode(new BaseUnitScreenSaverMode());
  }
  else if(m_stashedUsageMode)
  {
    restoreUsageMode(m_stashedUsageMode);
    m_stashedUsageMode = nullptr;
  }
}

void BaseUnit::respectUsageMode(const Setting *s)
{
  const BaseUnitUIMode *m = dynamic_cast<const BaseUnitUIMode *>(s);

  switch(m->get())
  {
    case BaseUnitUIModes::Play:
      setUsageMode(new BaseUnitPlayMode());
      break;

    case BaseUnitUIModes::ParameterEdit:
      setUsageMode(new BaseUnitEditMode());
      break;

    case BaseUnitUIModes::Banks:
      setUsageMode(new BaseUnitBanksMode());
      break;

    case BaseUnitUIModes::Presets:
      setUsageMode(new BaseUnitPresetsMode());
      break;
  }
}

void BaseUnit::indicateBlockingMainThread(bool onOff)
{
  m_lowerRibbon.indicateBlockingMainThread(onOff);
}

void BaseUnit::onTimeout()
{
  m_playPanel.onTimeout();
}

PlayPanel &BaseUnit::getPlayPanel()
{
  return m_playPanel;
}
