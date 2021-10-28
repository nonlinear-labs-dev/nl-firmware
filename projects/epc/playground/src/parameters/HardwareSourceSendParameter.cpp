#include <device-settings/Settings.h>
#include <presets/ParameterGroupSet.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-settings/midi/RoutingSettings.h>
#include "HardwareSourceSendParameter.h"
#include <presets/EditBuffer.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <parameters/PhysicalControlParameter.h>
#include <xml/Writer.h>

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

HardwareSourceSendParameter::HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, PhysicalControlParameter* sibling,
                                                         const ParameterId& id, const ScaleConverter* converter,
                                                         double def, int coarseDenominator, int fineDenominator,
                                                         OptRefSettings settings)
    : Parameter(pGroup, id, converter, def, coarseDenominator, fineDenominator)
    , m_settings(settings)
    , m_sibling{sibling}
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

void HardwareSourceSendParameter::sendParameterMessage() const
{
  if(auto eb = getParentEditBuffer())
    eb->getAudioEngineProxy().createAndSendParameterMessage<HardwareSourceSendParameter>(this);
}

void HardwareSourceSendParameter::onLocalChanged(const Setting* setting)
{
  if(auto localSetting = dynamic_cast<const GlobalLocalEnableSetting*>(setting))
  {
    auto local = localSetting->get();
    if(m_localIsEnabled != local)
    {
      m_localIsEnabled = local;
      invalidate();
    }
  }
}

void HardwareSourceSendParameter::onRoutingsChanged(const Setting* setting)
{
  if(auto routings = dynamic_cast<const RoutingSettings*>(setting))
  {
    auto state = routings->getState(getIndex(getID()), RoutingSettings::tAspectIndex::LOCAL);
    if(state != m_routingIsEnabled)
    {
      m_routingIsEnabled = state;
      invalidate();
    }
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

nlohmann::json HardwareSourceSendParameter::serialize() const
{
  auto param = Parameter::serialize();
  param.push_back({"is-enabled", isEnabled() });
  return param;
}

void HardwareSourceSendParameter::writeDocProperties(Writer& writer,
                                                     UpdateDocumentContributor::tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);
  writer.writeTextElement("local-enabled", std::to_string(isEnabled()));
}

bool HardwareSourceSendParameter::isEnabled() const
{
  return m_routingIsEnabled && m_localIsEnabled;
}

ReturnMode HardwareSourceSendParameter::getReturnMode() const
{
  return m_sibling->getReturnMode();
}

Layout* HardwareSourceSendParameter::createLayout(FocusAndMode focusAndMode) const
{
  return m_sibling->createLayout(focusAndMode);
}
