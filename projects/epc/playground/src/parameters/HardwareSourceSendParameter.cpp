#include <device-settings/Settings.h>
#include <presets/ParameterGroupSet.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-settings/midi/RoutingSettings.h>
#include "HardwareSourceSendParameter.h"

/*
 *
 *  - [ ] Wenn Local Off, bildet Send den physischen Status der HW-Sources ab, also das, was
    - [ ] nach draußen geschickt wird. Receive bildet die Werte ab, die an die AE weitergeleitet
werden (über MIDI empfangen oder über verstellte MCs bei bidirektionalen Sources).
Auf Send haben HW-Source und Encoder Einfluss
Ribbon LEDs zeigen den Send Status an, nicht den Receive.
Äquivalent zum Pedal, das auch dort steht, wo es physisch hinbewegt wurd
 *
 */

HardwareSourceSendParameter::HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, const ParameterId& id,
                                       const ScaleConverter* converter, double def, int coarseDenominator,
                                       int fineDenominator, OptRefSettings s)
    : Parameter(pGroup, id, converter, def, coarseDenominator, fineDenominator)
    , m_settings(s)
{
  if(m_settings.has_value())
  {
    auto settingsWrapper = m_settings.value();
    auto local = settingsWrapper.get().getSetting<GlobalLocalEnableSetting>();
    local->onChange(sigc::mem_fun(this, &HardwareSourceSendParameter::onLocalChanged));
    auto routings = settingsWrapper.get().getSetting<RoutingSettings>();
    routings->onChange(sigc::mem_fun(this, &HardwareSourceSendParameter::onRoutingsChanged));
  }
}

Glib::ustring HardwareSourceSendParameter::getLongName() const
{
  return getID().toString();
}

Glib::ustring HardwareSourceSendParameter::getShortName() const
{
  return getID().toString();
}

Glib::ustring HardwareSourceSendParameter::getInfoText() const
{
  return getID().toString();
}

void HardwareSourceSendParameter::onLocalChanged(const Setting* setting)
{
  if(auto localSetting = dynamic_cast<const GlobalLocalEnableSetting*>(setting))
  {
    m_localIsEnabled = localSetting->get();
  }
}

void HardwareSourceSendParameter::onRoutingsChanged(const Setting* setting)
{
  if(auto routings = dynamic_cast<const RoutingSettings*>(setting))
  {
    m_routingIsEnabled = routings->getState(getIndex(getID()), RoutingSettings::tAspectIndex::LOCAL);
  }
}

RoutingSettings::tRoutingIndex HardwareSourceSendParameter::getIndex(const ParameterId& id)
{
  using tIdx = RoutingSettings::tRoutingIndex;

  constexpr auto to = [](PlaceholderIDS i) {
    return static_cast<int>(i);
  };

  switch(id.getNumber())
  {
    case to(PlaceholderIDS::Bender_Send):
      return tIdx::Bender;
    case to(PlaceholderIDS::Aftertouch_Send):
      return tIdx::Aftertouch;
    case to(PlaceholderIDS::Pedal1_Send):
      return tIdx::Pedal1;
    case to(PlaceholderIDS::Pedal2_Send):
      return tIdx::Pedal2;
    case to(PlaceholderIDS::Pedal3_Send):
      return tIdx::Pedal3;
    case to(PlaceholderIDS::Pedal4_Send):
      return tIdx::Pedal4;
    case to(PlaceholderIDS::Ribbon1_Send):
      return tIdx::Ribbon1;
    case to(PlaceholderIDS::Ribbon2_Send):
      return tIdx::Ribbon2;
  }
  nltools_assertNotReached();
}
