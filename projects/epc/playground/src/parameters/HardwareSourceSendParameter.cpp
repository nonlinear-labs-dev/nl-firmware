#include <device-settings/Settings.h>
#include <presets/ParameterGroupSet.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-settings/midi/RoutingSettings.h>
#include "HardwareSourceSendParameter.h"
#include <presets/EditBuffer.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <parameters/PhysicalControlParameter.h>
#include <xml/Writer.h>

HardwareSourceSendParameter::HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, PhysicalControlParameter* sibling,
                                                         const ParameterId& id, const ScaleConverter* converter,
                                                         double def, int coarseDenominator, int fineDenominator,
                                                         Settings* settings)
    : Parameter(pGroup, id, converter, def, coarseDenominator, fineDenominator)
    , m_sibling{sibling}
    , m_settings(settings)
{
  if(m_settings)
  {
    auto local = settings->getSetting<GlobalLocalEnableSetting>();
    local->onChange(sigc::mem_fun(this, &HardwareSourceSendParameter::onLocalChanged));
    auto routings = settings->getSetting<RoutingSettings>();
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
  param.push_back({"is-enabled", isLocalEnabled() });
  return param;
}

void HardwareSourceSendParameter::writeDocProperties(Writer& writer,
                                                     UpdateDocumentContributor::tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);
  writer.writeTextElement("local-enabled", std::to_string(isLocalEnabled()));
}

bool HardwareSourceSendParameter::isLocalEnabled() const
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

PhysicalControlParameter* HardwareSourceSendParameter::getSiblingParameter() const
{
  return m_sibling;
}
