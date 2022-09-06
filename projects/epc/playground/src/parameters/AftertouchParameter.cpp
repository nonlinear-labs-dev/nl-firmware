#include "AftertouchParameter.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/PlayControlParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <glib.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-settings/midi/RoutingSettings.h>
#include <presets/EditBuffer.h>
#include <libundo/undo/Scope.h>

ReturnMode AftertouchParameter::getReturnMode() const
{
  return ReturnMode::Zero;
}

bool AftertouchParameter::isLocalEnabled() const
{
  if(auto eb = getParentEditBuffer())
  {
    auto& s = eb->getSettings();
    const auto globalLocalState = s.getSetting<GlobalLocalEnableSetting>()->get();
    const auto setting = s.getSetting<RoutingSettings>();
    const auto state
        = setting->getState(RoutingSettings::tRoutingIndex::Aftertouch, RoutingSettings::tAspectIndex::LOCAL);
    return state && globalLocalState;
  }
  return false;
}

tControlPositionValue AftertouchParameter::getDefValueAccordingToMode() const
{
  return 0;
}

void AftertouchParameter::onLocalEnableChanged(bool localEnableState)
{
  auto scope = UNDO::Scope::startTrashTransaction();

  if(localEnableState)
  {
    auto oldSendPos = getSendParameter()->getControlPositionValue();
    getSendParameter()->setCPFromSetting(scope->getTransaction(), getDefValueAccordingToMode());
    PhysicalControlParameter::setCPFromSetting(scope->getTransaction(), oldSendPos);
  }
  else
  {
    getSendParameter()->setCPFromSetting(scope->getTransaction(), getControlPositionValue());
    PhysicalControlParameter::setCPFromSetting(scope->getTransaction(), getDefValueAccordingToMode());
  }
}
