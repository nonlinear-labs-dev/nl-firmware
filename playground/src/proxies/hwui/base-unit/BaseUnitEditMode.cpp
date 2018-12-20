#include "BaseUnitEditMode.h"
#include "Application.h"
#include "device-settings/Settings.h"
#include "device-settings/ParameterEditModeRibbonBehaviour.h"
#include "proxies/lpc/LPCProxy.h"
#include "groups/HardwareSourcesGroup.h"

void BaseUnitEditMode::toggleTouchBehaviour()
{
  auto p = Application::get().getLPCProxy();

  if(p->getLastTouchedRibbonParameterID() == HardwareSourcesGroup::getUpperRibbonParameterID())
    toggleParameterEditModeRibbonBehaviour();
  else
    super::toggleTouchBehaviour();
}

void BaseUnitEditMode::toggleParameterEditModeRibbonBehaviour()
{
  Application::get().getSettings()->getSetting<ParameterEditModeRibbonBehaviour>()->inc(1, true);
}
